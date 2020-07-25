#include"../include/triangle.hpp"
#include<iostream>
#include"../include/math.hpp"

namespace Render {

// 用这个函数进行lesson2中的人脸渲染时，会有很多空洞，应该是很多三角形没有渲染出来，可能有些情况的三角形没有渲染出来，需要之后再修改一下
void triangle_bymyself(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
    // 通过y值对三角形三个顶点排序。
    Vec2i highest, mid, lowest;
    if (t0.v > t1.v) {
        highest.u = t0.u; highest.v = t0.v;
        mid.u = t1.u; mid.v = t1.v;
    }
    else {
        highest.u = t1.u; highest.v = t1.v;
        mid.u = t0.u; mid.v = t0.v;
    }
    if (highest.v > t2.v) {
        if (mid.v > t2.v) {
            lowest.u = t2.u; lowest.v = t2.v;
        }
        else {
            lowest.u = mid.u; lowest.v = mid.v;
            mid.u = t2.u; mid.v = t2.v;
        }
    }
    else {
        lowest.u = mid.u; lowest.v = mid.v;
        mid.u = highest.u; mid.v = highest.v;
        highest.u = t2.u; highest.v = t2.v;
    }

    // 递减y，用与Bresenham相同的方法计算x的递增量,当两侧的y同时递增时，画一条水平直线。
    // 这种做法的三角形边有可能与Bresenham算法画出的轮廓不一样，因为这么做的轮廓一定是从y更高的点画到y更低的点，而我Bresenham算法实现的方法是从x更低的点画到x更高的点，只有顺序相同时，轮廓线才相同，因此如果想保证相同，需要将lineBresenham实现改成从y更高的点画到y更低的点。
    int xBegin = highest.u;
    int yBegin = highest.v;
    int xCur1 = xBegin;
    int xCur2 = xBegin;
    int yCur = yBegin;
    int yEnd = mid.v;
    int xEnd = mid.u;
    int dx1 = abs(highest.u - lowest.u);
    int dx2 = abs(highest.u - mid.u);
    int dy1 = abs(highest.v - lowest.v);
    int dy2 = abs(highest.v - mid.v);
    bool steep1 = false, steep2 = false;
    if (dy1 > dx1) {
        steep1 = true;
    }
    if (dy2 > dx2) {
        steep2 = true;
    }
    int xIncr1 = lowest.u < highest.u ? -1 : 1;
    int xIncr2 = mid.u < highest.u ? -1 : 1;
    int error1, error2, incr1, incr2;
    if (steep1) {
        error1 = -dy1;
        incr1 = 2 * dx1;
    }
    else {
        error1 = -dx1;
        incr1 = 2 * dy1;
    }
    if (steep2) {
        error2 = -dy2;
        incr2 = 2 * dx2;
    }
    else {
        error2 = -dx2;
        incr2 = 2 * dy2;
    }
    // 考虑三角形的上边是否是水平边，如果是水平边（最高点和第二高点高度相同），跳过第一步的填充过程（即对最高点和第二高的点之间的填充）
    if(dy2 == 0) {
        xCur2 = mid.u;
    }
    else {
        while (yCur > yEnd) {   
            line(xCur1, yCur, xCur2, yCur, image, color, Render::LineMethod::BRESENHAM);
            if (steep1) { // 当斜率大于1的时候，每一次低递减yCur,xCur可能递增（+xIncr）也可能不变，但是最多只会递增一次，因此从yCur的角度实现Bresenham算法。
                error1 += incr1;
                if (error1 >= 0) {
                    xCur1 += xIncr1;
                    error1 -= 2 * dy1;
                }
            }
            else {   // 当斜率大于1的时候，每一次递减yCur,xCur一定会递增，但是可能会递增多个值，不只递增一次，因此从xCur的角度实现Bresenham算法。
                while (true) {
                    error1 += incr1;
                    xCur1 += xIncr1;
                    if (error1 >= 0) {
                        error1 -= 2 * dx1;
                        break;
                    }
                }
            }
            if (steep2) { // 当斜率大于1的时候，每一次低递减yCur,xCur可能递增（+xIncr）也可能不变，但是最多只会递增一次，因此从yCur的角度实现Bresenham算法。
                error2 += incr2;
                if (error2 >= 0) {
                    xCur2 += xIncr2;
                    error2 -= 2 * dy2;
                }
            }
            else {
                while (true) {
                    error2 += incr2;
                    xCur2 += xIncr2;   // 每一次循环都要递增一次xCur,因为这里相当于在xCur的角度看yCur，当yCur递减时跳出循环，看xCur递增了多少
                    if (error2 >= 0) {
                        error2 -= 2 * dx2;
                        break;
                    }
                }
            }
            yCur--;
        }
    }
    yEnd = lowest.v;
    dx2 = abs(mid.u - lowest.u);
    dy2 = abs(mid.v - lowest.v);
    steep2 = dy2 > dx2 ? true : false;
    xIncr2 = lowest.u < mid.u ? -1 : 1;
    if (steep2) {
        error2 = -dy2;
        incr2 = 2 * dx2;
    }
    else {
        error2 = -dx2;
        incr2 = 2 * dy2;
    }
    //std::cout << yCur << " " << yEnd << std::endl;
    if (dy2 == 0) {
        line(xCur1, yCur, xCur2, yCur, image, color, Render::LineMethod::BRESENHAM);
        return;
    }
    while (yCur >= yEnd) {
        line(xCur1, yCur, xCur2, yCur, image, color, Render::LineMethod::BRESENHAM);
        if (steep1) { // 当斜率大于1的时候，每一次低递减yCur,xCur可能递增（+xIncr）也可能不变，但是最多只会递增一次，因此从yCur的角度实现Bresenham算法。
            error1 += incr1;
            if (error1 >= 0) {
                xCur1 += xIncr1;
                error1 -= 2 * dy1;
            }
        }
        else {   // 当斜率大于1的时候，每一次递减yCur,xCur一定会递增，但是可能会递增多次，不只递增一次，因此从xCur的角度实现Bresenham算法。
            while (true) {
                error1 += incr1;
                xCur1 += xIncr1;
                if (error1 >= 0) {
                    error1 -= 2 * dx1;
                    break;
                }
            }
        }
        if (steep2) { // 当斜率大于1的时候，每一次低递减yCur,xCur可能递增（+xIncr）也可能不变，但是最多只会递增一次，因此从yCur的角度实现Bresenham算法。
            error2 += incr2;
            if (error2 >= 0) {
                xCur2 += xIncr2;
                error2 -= 2 * dy2;
            }
        }
        else {
            while (true) {
                error2 += incr2;
                xCur2 += xIncr2;   // 每一次循环都要递增一次xCur,因为这里相当于在xCur的角度看yCur，当yCur递减时跳出循环，看xCur递增了多少
                if (error2 >= 0) {
                    error2 -= 2 * dx2;
                    break;
                }
            }
        }
        yCur--;
    }
}

void triangle_bylesson(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) { 
    if (t0.y==t1.y && t0.y==t2.y) return; // I dont care about degenerate triangles 
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
    if (t0.y>t1.y) std::swap(t0, t1); 
    if (t0.y>t2.y) std::swap(t0, t2); 
    if (t1.y>t2.y) std::swap(t1, t2); 
    int total_height = t2.y-t0.y; 
    for (int i=0; i<total_height; i++) { 
        bool second_half = i>t1.y-t0.y || t1.y==t0.y; 
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y; 
        float alpha = (float)i/total_height; 
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here 
        Vec2i A =               t0 + (t2-t0)*alpha; 
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta; 
        if (A.x>B.x) std::swap(A, B); 
        for (int j=A.x; j<=B.x; j++) { 
            image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y 
        } 
    } 
}


void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {

    int maxX = image.get_width() - 1;
    int minX = 0;
    int maxY = image.get_height() - 1;
    int minY = 0;
    
    maxX = std::min(maxX, std::max(std::max(t0.x, t1.x), t2.x));
    minX = std::max(minX, std::min(std::min(t0.x, t1.x), t2.x));
    maxY = std::min(maxY, std::max(std::max(t0.y, t1.y), t2.y));
    minY = std::max(minY, std::min(std::min(t0.y, t1.y), t2.y));


    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Vec3f barycentricPoint = barycentric(std::vector<Vec2i>{t0, t1, t2}, Vec2i(x, y));
            if (barycentricPoint.x >= 0 && barycentricPoint.y >= 0 && barycentricPoint.z >= 0) {
                image.set(x, y, color);
            }
        }
    }
}


}