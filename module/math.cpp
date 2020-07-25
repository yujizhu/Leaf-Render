#include"math.hpp"

namespace Render {

Vec3f crossProduct(Vec3i l, Vec3i r) {
    return Vec3f(l.y * r.z - r.y * l.z, l.z * r.x - l.x * r.z, l.x * r.y - r.x * l.y);
}

Vec3f barycentric(const std::vector<Vec2i>& trianglePoint, Vec2i P) {
    Vec2i ab = trianglePoint[1] - trianglePoint[0];
    Vec2i ac = trianglePoint[2] - trianglePoint[0];
    Vec2i pa = trianglePoint[0] - P;
    Vec3f u = crossProduct(Vec3i(ab.x, ac.x, pa.x), Vec3i(ab.y, ac.y, pa.y));

    if (std::abs(u.y)<1) return Vec3f(-1,1,1);

    return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
}

}

