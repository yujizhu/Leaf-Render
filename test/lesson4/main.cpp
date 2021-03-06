#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "line.hpp"
#include "iostream"
#include "triangle.hpp"
#include <climits>
#include <limits>
#include "camera.hpp"
#include "math.hpp"

using namespace Render;
using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model *model = NULL;


void line_a(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
}

void triangle_a(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) { 
    line(t0.x, t0.y, t1.x, t1.y, image, color, Render::LineMethod::BRESENHAM); 
    line(t1.x, t1.y, t2.x, t2.y, image, color, Render::LineMethod::BRESENHAM); 
    line(t2.x, t2.y, t0.x, t0.y, image, color, Render::LineMethod::BRESENHAM); 
}

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }
    const int width  = 800;
    const int height = 800;
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage texture;
    Camera cam(Vec3f(0, 0, 0), Vec3f(0, 0, 5), Vec3f(0, 0, 1), 800, 800);
    Matrixf projection = cam.getPerspectiveMatrix();
    Matrixf viewPort = cam.getViewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, 255);
    bool oState = texture.read_tga_file("african_head_diffuse.tga");
    if (!oState) {
        cout << "fail!" << endl;
    }
    Matrixf mat1{{1, 2, 3}, {4, 5, 6}};
    Matrixf mat2{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::cout << mat1 * mat2 << std::endl;
    vector<vector<int>> mmmm = {{1, 2, 3}, {4, 5, 6}};
    /*
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f world_coords = model->vert(face[j]);
            screen_coords[j] = Vec2i((world_coords.x + 1.) * width / 2., (world_coords.y + 1.) * height / 2.);
        }
        triangle_bymyself(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
    }
    */

    Vec3f light_dir(0,0,-1);
    vector<vector<float>> zBuffer(width, vector<float>(height, -std::numeric_limits<float>::max()));  // 这里之前存的int型，应该是错了。  有一个问题这里为什么用-std::numeric_limits<float>::max()而不用std::numeric_limits<float>::min()?因为std::numeric_limits<float>::max() = 3.40282e+38 而std::numeric_limits<float>::min() = 1.17549e-38，并不是我一开始以为的float能存的最小的数，反而是一个正数，这是因为它存储的就是float能表示的最小的正数（应该和float的二进制表示有关），因此如果想要保存float所能表示的最小的数（最小的负数），应该使用-std::numeric_limits<float>::max()，此外C++11中添加了lowest()函数，其值与-std::numeric_limits<float>::max()相同。
    //std::cout << -std::numeric_limits<float>::max() << "      " << std::numeric_limits<float>::lowest() << std::endl;
    texture.flip_vertically();   // 这里需要先对原图片进行翻转，因为图片以左下角为原点，而代码内部处理矩阵时以左上角作为原点。
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<std::vector<int>> face = model->face(i); 
        Vec3i screen_coords[3]; 
        Vec3f world_coords[3]; 
        Vec3f world_coords_t[3];
        Vec2f text_coords[3];
        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j][0]);
            Vec2f t = model->text(face[j][1]);
            //screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
            //Matrixf aaa = viewPort *projection *homoCoordinateTransformation(v);
            screen_coords[j] = invHomoCoordinateTransformation(viewPort * projection * homoCoordinateTransformation(v));
            world_coords[j] = Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z); // 这里改了，之前写的有点问题，没有转成int型
            //std::cout << world_coords[j] << std::endl;
            world_coords_t[j] = v;
            text_coords[j] = t;
        }
        Vec3f n = invHomoCoordinateTransformation(viewPort * projection * homoCoordinateTransformation(v)); 
        n.normalize(); 
        float intensity = n*light_dir;
        if (intensity > 0) { // 如果点乘的结果为负，说明光线来自多边形的后方，因此需要去掉这个三角形。
            //triangle_bylesson(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));  // 光线与平面越垂直，平面的光强度越大。因此用点乘计算这个垂直的程度。（在相同的光强度下，当多边形与光的方向正交时，其照度最高。）
            //triangle(vector<Vec3f>{world_coords[0], world_coords[1], world_coords[2]}, zBuffer, image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
            //triangle(vector<Vec3f>{world_coords[0], world_coords[1], world_coords[2]}, vector<Vec2f>{text_coords[0], text_coords[1], text_coords[2]}, zBuffer, image, texture);
            triangle(vector<Vec3f>{screen_coords[0], screen_coords[1], screen_coords[2]}, vector<Vec2f>{text_coords[0], text_coords[1], text_coords[2]}, zBuffer, image, texture);
        }
    }

    /*
    Vec2i t0[3] = {Vec2i(50, 70),   Vec2i(50, 160),  Vec2i(70, 70)};    //30 80
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 
    triangle_bymyself(t0[0], t0[1], t0[2], image, red);

    //line(50, 160, 30, 80, image, red, Render::LineMethod::BRESENHAM);
    //line(30, 80, 50, 160, image, red, Render::LineMethod::BRESENHAM);
    //triangle_bymyself(Vec2i(10 + 80, 70),   Vec2i(50 + 80, 160),  Vec2i(70 + 80, 80), image, red);
    //triangle(t1[0], t1[1], t1[2], image, white);
    //triangle(t2[0], t2[1], t2[2], image, green);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    cout << "end" << endl;
    */
    
    image.flip_vertically(); 
    image.write_tga_file("output.tga");
    delete model;
    cout << "end" << endl;
    int aa;
    cin >> aa;


    return 0;
}

