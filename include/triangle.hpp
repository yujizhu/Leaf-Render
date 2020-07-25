#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include"line.hpp"
#include"../test/geometry.h"
#include<vector>

namespace Render {


void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color);
void triangle_bylesson(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);
void triangle_bymyself(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);

}

#endif

