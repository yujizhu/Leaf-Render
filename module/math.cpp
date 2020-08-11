#include"math.hpp"

namespace Render {

Vec3f crossProduct(Vec3i l, Vec3i r) {
    return Vec3f(l.y * r.z - r.y * l.z, l.z * r.x - l.x * r.z, l.x * r.y - r.x * l.y);
}

Vec3f crossProduct(Vec3f l, Vec3f r) {
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

Vec3f barycentric(const std::vector<Vec3f>& trianglePoint, Vec3f P) {
    Vec3f ab = trianglePoint[1] - trianglePoint[0];
    Vec3f ac = trianglePoint[2] - trianglePoint[0];
    Vec3f pa = trianglePoint[0] - P;
    Vec3f u = crossProduct(Vec3f(ab.x, ac.x, pa.x), Vec3f(ab.y, ac.y, pa.y));

    if (std::abs(u.y)>1e-2) return Vec3f(1.f-(u.x+u.y)/u.z, u.x/u.z, u.y/u.z);     // 这块和教程代码不太一样，因为求重心的公式用的不太一样。这里u.x和u.y交换后，结果是一样的，为什么？

    return Vec3f(-1,1,1); 
}

}

