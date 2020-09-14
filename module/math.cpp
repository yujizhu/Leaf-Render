#include"math.hpp"

namespace Render {

Matrixf homoCoordinateTransformation(Vec3f point) {
    Matrixf result(4, 1);
    result[0][0] = point.x;
    result[1][0] = point.y;
    result[2][0] = point.z;
    result[3][0] = 1;
    return result;
}

Vec3f invHomoCoordinateTransformation(const Matrixf& matrix) {
    assert(matrix.rows() == 4 && matrix.cols() == 1);
    return Vec3f(matrix[0][0] / matrix[3][0], matrix[1][0] / matrix[3][0], matrix[2][0] / matrix[3][0]);
}

Vec3f crossProduct(Vec3i l, Vec3i r) {
    return Vec3f(l.y * r.z - r.y * l.z, l.z * r.x - l.x * r.z, l.x * r.y - r.x * l.y);
}

Vec3f crossProduct(Vec3f l, Vec3f r) {
    return Vec3f(l.y * r.z - r.y * l.z, l.z * r.x - l.x * r.z, l.x * r.y - r.x * l.y);
}

template<typename T>
Vec3<T> crossProduct(Vec3<T> l, Vec3<T> r) {
    return Vec3<T>(l.y * r.z - r.y * l.z, l.z * r.x - l.x * r.z, l.x * r.y - r.x * l.y);
}

Vec3f barycentric(const std::vector<Vec2i>& trianglePoint, Vec2i P) {
    Vec2i ab = trianglePoint[1] - trianglePoint[0];
    Vec2i ac = trianglePoint[2] - trianglePoint[0];
    Vec2i pa = trianglePoint[0] - P;
    Vec3f u = crossProduct(Vec3i(ab.x, ac.x, pa.x), Vec3i(ab.y, ac.y, pa.y));

    if (std::abs(u.y)<1) return Vec3f(-1,1,1);

    return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
}

//最早的条件写错了，这个实现和最下面的本质上没有不同，后一种实现只是将ab和ac换了个位置。
Vec3f barycentric(const std::vector<Vec3f>& trianglePoint, Vec3f P) {   
    Vec3f ab = trianglePoint[1] - trianglePoint[0];
    Vec3f ac = trianglePoint[2] - trianglePoint[0];
    Vec3f pa = trianglePoint[0] - P;
    Vec3f u = crossProduct(Vec3f(ab.x, ac.x, pa.x), Vec3f(ab.y, ac.y, pa.y));

    if (std::abs(u.z)>1e-2) return Vec3f(1.f-(u.x+u.y)/u.z, u.x/u.z, u.y/u.z);     // 最早这里的条件写错了，之前一直用abs(u.y)>1e-2来做，实际上要判断u.z是否不为0。

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

} // end namespace Render

