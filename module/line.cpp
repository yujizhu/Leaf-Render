#include"line.hpp"
#include"tgaimage.h"
#include<cmath>
#include<iostream>

using namespace std;

namespace Render {


// 原Bresenham算法只能用于斜率在0至1的直线生成，生成斜率大于1的直线时，可以将x和y交换一下再生成。
// 第一次实现的时候没有考虑斜率小于1的情况，当斜率小于1时，将y的每次增量改为-1即可。
// 有一个问题：使用Bresenham算法画课程样例时画出的竖线不直，而直接用斜率画的就很直，为什么？
// 还有一个问题：关于生成线的方向的问题，可以有多种做法，如果以x作为主要迭代方向，可以从x较低的点向x较高的点生成直线；也可以根据传入函数的参数
//              顺序生成直线，如果根据传入函数的参数顺序生成直线的话，颠倒两个点在函数中的顺序，生成的直线也会不同，因此在实现方法上要考虑清楚按什么顺序生成直线。
// 优化方法：用移位运算实现加法
void lineBresenham(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
    int dx = std::abs(x1 - x0);   // 在推导过程中，由于x0,y0,x1,y1都是整型，因此dx,dy也一定是整型，相应的d也是整型，判断y的递增时只判断d的正负即可。
    int dy = std::abs(y1 - y0);
    int m = image.get_width();
    int n = image.get_height();
    bool steep = false;
    if (dy > dx) {
        steep = true;
        std::swap(dx, dy);
        std::swap(x0, y0);
        std::swap(x1, y1);
    }   
    int d = 2 * dy - dx;
    int xBegin, xEnd, yBegin, yEnd;
    int incr1 = 2 * dy;
    int incr2 = 2 * (dy - dx);
    
    if (x0 <= x1) {
        xBegin = x0; yBegin = y0;
        xEnd = x1; yEnd = y1;
    }
    else {
        xBegin = x1; yBegin = y1;
        xEnd = x0; yEnd = y0;
    }
    
    // 用于按照传入参数的顺序生成直线的算法实现
    /*   // 用下面注释掉的这种做法，而不像上面这样根据x0和x1的大小交换两者会有一个问题，下面这么做永远会从line函数参数的第一个点画到第二个点，如果line函数中这两个点的参数位置交换一下，画出来的直线是不一样的。
    xBegin = x0; yBegin = y0;
    xEnd = x1; yEnd = y1;
    int xIncr = 1;
    if (x0 > x1) {
        xIncr = -1;
    }
    int xCur, yCur;
    */

    int yIncr = yBegin > yEnd ? -1 : 1;
    
    // 向下面这样将分支放到循环的外面的做法是常见的优化手段。
    if (steep) {  // 之前的做法是将这个分支判断放在循环里进行的，将分支判断放在循环里可以减少代码的行数，但是每一次循环都会进行一次判断，耗费了时间，将分支判断放在循环外卖呢能够进行加速。此外这样做能使编译器的优化做得更好。
        for (int xCur = xBegin, yCur = yBegin; xCur <= xEnd; xCur++) {
            if (yCur > m || xCur > n) {
                break;   // 如果直线出了图片边界，停止直线生成。
            }
            //cout << yCur << " " << xCur << endl;
            image.set(yCur, xCur, color);  // 将之前交换的x和y交换回来。
            if (d < 0) {
                d += incr1;
            }
            else {
                yCur += yIncr;
                d += incr2;
            }
        }
    }
    else {
        for (int xCur = xBegin, yCur = yBegin; xCur <= xEnd; xCur++) {
            if (yCur > m || xCur > n) {
                break;   // 如果直线出了图片边界，停止直线生成。
            }
            //cout << xCur << " " << yCur << endl;
            image.set(xCur, yCur, color);
            if (d < 0) {
                d += incr1;
            }
            else {
                yCur += yIncr;
                d += incr2;
            }
        }
    }
    /*  // 用于按照传入参数的顺序生成直线的算法实现
    if (steep) {
        image.set(yCur, xCur, color);
    }
    else {
        image.set(xCur, yCur, color);
    }
    */
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color, enum LineMethod method) {
    
    switch (method) {
        case LineMethod::BRESENHAM :
            lineBresenham(x0, y0, x1, y1, image, color);
    }

}




}



