#include"../test/geometry.h"

namespace Render {

// 模板的特化需要放在cpp文件中，因为模板特化已经和普通的函数一样了，不能在多个编译单元内重复定义（这段是我猜测的，需要再查查资料，将下面的特化放在.h文件中会出现redefinition的问题。
template <>
template <>
Vec3<int>::Vec3(const Vec3<float>& r) : x(int(r.x + 0.5)), y(int(r.y + 0.5)), z(int(r.z + 0.5)) {} // 这里不能写成 Vec3<int>::Vec3<int>，因为这里Vec3表示构造函数的函数名，并非是injected name（注入类名）

template <>
template <>
Vec3<float>::Vec3(const Vec3<int>& r) : x(float(r.x)), y(float(r.y)), z(float(r.z)) {} // 这里不能写成 Vec3<float>::Vec3<float>，因为这里Vec3表示构造函数的函数名，并非是injected name（注入类名）


}; // end namespace Render
