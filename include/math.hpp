#ifndef MATH_HPP
#define MATH_HPP

#include<iostream>
#include"../test/geometry.h"
#include<vector>

namespace Render {


Vec3f crossProduct(Vec3i l, Vec3i r);
Vec3f barycentric(const std::vector<Vec2i>& trianglePoint, Vec2i P);
Vec3f barycentric(const std::vector<Vec3f>& trianglePoint, Vec3f P);
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

}


#endif