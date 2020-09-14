#include"camera.hpp"

using namespace std;

namespace Render {

Matrixf Camera::getRotateTransform() {
    Vec3f z = (lookfrom - lookat).normalize();
    //std::cout << "z: " << z << std::endl;
    Vec3f x = crossProduct(up, z).normalize();
    //std::cout << "x: " << x << std::endl;
    Vec3f y = crossProduct(z, x).normalize();
    //std::cout << "y: " << y << std::endl;
    Matrixf Minv{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    Matrixf Tr{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    
    for (int i = 0; i < 3; i++) {
        Minv[0][i] = x.raw[i];
        Minv[1][i] = y.raw[i];
        Minv[2][i] = z.raw[i];
        Tr[i][3] = -lookat.raw[i];
    }
    //std::cout << "Minv:" << std::endl;
    //std::cout << Minv << std::endl;
    //std::cout << "Tr:" << std::endl;
    //std::cout << Tr << std::endl;
    return Minv * Tr;
}

} // end namespace Render
