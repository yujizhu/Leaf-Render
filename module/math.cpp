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

//下面的第一种求重心坐标的实现不如第二种理想，第一种来自我自己的推导，为什么？ 
Vec3f barycentric(const std::vector<Vec3f>& trianglePoint, Vec3f P) {   // 这个重心坐标求法和下面的有什么不同？
    Vec3f ab = trianglePoint[1] - trianglePoint[0];
    Vec3f ac = trianglePoint[2] - trianglePoint[0];
    Vec3f pa = trianglePoint[0] - P;
    Vec3f u = crossProduct(Vec3f(ab.x, ac.x, pa.x), Vec3f(ab.y, ac.y, pa.y));

    if (std::abs(u.y)>1e-2) return Vec3f(1.f-(u.x+u.y)/u.z, u.x/u.z, u.y/u.z);     // 这块和教程代码不太一样，因为求重心的公式用的不太一样。这里u.x和u.y交换后，结果是一样的，为什么？

    return Vec3f(-1,1,1); 
}

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i].raw[0] = C.raw[i]-A.raw[i];
        s[i].raw[1] = B.raw[i]-A.raw[i];
        s[i].raw[2] = A.raw[i]-P.raw[i];
    }
    Vec3f u = crossProduct(s[0], s[1]);
    if (std::abs(u.raw[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

}

