#ifndef LINE_HPP
#define LINE_HPP

#include"tgaimage.h"

namespace Render {

enum class LineMethod{
    BRESENHAM = 0
};

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color, enum LineMethod method);


}

#endif