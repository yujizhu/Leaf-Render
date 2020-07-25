#include"tgaimage.h"
#include"line.hpp"
#include<iostream>

using namespace Render;
using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main() {
    TGAImage image(800, 800, TGAImage::RGB);
    // Render::line(13, 20, 80, 40, image, white, Render::LineMethod::BRESENHAM);
    //line(370, 312, 349, 353, image, white, Render::LineMethod::BRESENHAM);
    //cout << "aa" << endl;
    // line(50, 160, 150, 80, image, white, Render::LineMethod::BRESENHAM); 
    //line(20, 13, 40, 80, image, red, Render::LineMethod::BRESENHAM); 
    //line(80, 40, 13, 20, image, red, Render::LineMethod::BRESENHAM);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    int aa;
    cin >> aa;


    return 0;
}