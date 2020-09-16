#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include"line.hpp"
#include"../test/geometry.h"
#include<vector>

namespace Render {

void triangle(const std::vector<Vec3f> &t, std::vector<std::vector<int>>& zBuffer, TGAImage &image, TGAColor color);
void triangle(const std::vector<Vec3f> &v, const std::vector<Vec2f> &t, std::vector<std::vector<int>> &zBuffer, TGAImage &image, TGAImage &texture);
void triangle_standard(Vec3i t0, Vec3i t1, Vec3i t2, Vec2f ity0, Vec2f ity1, Vec2f ity2, std::vector<std::vector<int>>& zBuffer, TGAImage& image, TGAImage& texture);
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);
void triangle_bylesson(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);
void triangle_bymyself(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);


}

#endif

