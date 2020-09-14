#ifndef CAMERA_HPP
#define CAMERA_HPP

#include"../test/geometry.h"
#include"math.hpp"


// 需不需要对重复调用get_函数用缓存技术。
// 需不需要对camera添加moving函数。
// 这个框架认为相机位于所在摄影坐标系的z轴上，便于进行投影变换。

namespace Render {

class Camera {
  public:
    Camera(Vec3f lookatPara, Vec3f lookfromPara, Vec3f upPara, int widthPara, int heightPara) 
      : lookat(lookatPara), lookfrom(lookfromPara), up(upPara), width(widthPara), height(heightPara) {
        
    }
    inline Matrixf getPerspectiveMatrix() {
      Matrixf result{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
      result[3][2] = -1/(lookat - lookfrom).norm();
      return result;
    }
    Matrixf getRotateTransform();
    
    inline Matrixf getViewport(int x, int y, int w, int h, int depth) {
      Matrixf m{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
      m[0][3] = x + w / 2.f;
      m[1][3] = y + h / 2.f;
      m[2][3] = depth / 2.f;

      m[0][0] = w / 2.f;
      m[1][1] = h / 2.f;
      m[2][2] = depth / 2.f;

      return m;
    }
    
  private:
    Vec3f lookat;    // 相机所在的空间坐标
    Vec3f lookfrom;  // 相机所指向的点坐标，即经过旋转变换后的原点坐标
    Vec3f up;        // 相机的“向上”向量
    int width;
    int height;
};

} // end namespace Render

#endif