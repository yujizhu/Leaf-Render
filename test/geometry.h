#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <vector>
#include <cassert>
#include <initializer_list>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class t> struct Vec2 {
	union {
		struct {t u, v;};
		struct {t x, y;};
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u),v(_v) {}
	inline Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(u+V.u, v+V.v); }
	inline Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(u-V.u, v-V.v); }
	inline Vec2<t> operator *(float f)          const { return Vec2<t>(u*f, v*f); }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
	union {
		struct {t x, y, z;};
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x),y(_y),z(_z) {}
	template <typename u>
	Vec3(const Vec3<u>& r);
	inline Vec3<t> operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline Vec3<t> operator +(const Vec3<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
	inline Vec3<t> operator -(const Vec3<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
	inline Vec3<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
	inline t       operator *(const Vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
	float norm () const { return std::sqrt(x*x+y*y+z*z); }
	Vec3<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

template <>
template <>
Vec3<int>::Vec3(const Vec3<float> &r); // 这里不能写成 Vec3<int>::Vec3<int>，因为这里Vec3表示构造函数的函数名，并非是injected name（注入类名）

template <>
template <>
Vec3<float>::Vec3(const Vec3<int> &r); // 这里不能写成 Vec3<float>::Vec3<float>，因为这里Vec3表示构造函数的函数名，并非是injected name（注入类名）

template <typename T>
class Matrix {
  public:
	Matrix(int m, int n) : row(m), col(n), mat(std::vector<std::vector<T>>(row, std::vector<T>(col, 0))) {}
	Matrix(const std::initializer_list<std::vector<T>>& initList) : mat(initList) {  // 这里必须使用std::initializer_list<std::vector<T>>类型进行初始化，不能使用std::initializer_list<std::initializer_list<T>>进行初始化。
		row = mat.size();
		if (row > 0) {
			col = mat[0].size();
		}
		else {
			col = 0;
		}
	}
	inline const std::vector<T>& operator[](int i) const { return mat[i]; } // 注意这里必须要添加一个const版本，且返回值必须为const类型。
	inline std::vector<T> &operator[](int i) { return mat[i]; }
	Matrix operator*(const Matrix& r) const;
	Matrix inverse();
	Matrix transpose();
	inline int rows() const { return row; }
	inline int cols() const { return col; }

  private:
	int row;
	int col;
	std::vector<std::vector<T>> mat;
};

typedef Matrix<int> Matrixi;
typedef Matrix<float> Matrixf;
typedef Matrix<double> Matrixd;

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& r) const {
	//std::cout << col << " " << r.row << std::endl;
	//std::cout << r << std::endl;
	//std::cout << std::endl;
	assert(col == r.row);
	Matrix result(row, r.col);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < r.col; j++) {
			for (int k = 0; k < col; k++) {
				result[i][j] += mat[i][k] * r.mat[k][j];
			}
		}
	}
	return result;
}

template<typename T>
std::ostream& operator<<(std::ostream& s, Matrix<T> mat) {
	int row = mat.rows();
	int col = mat.cols();
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col - 1; j++) {
			s << mat[i][j] << " ";
		}
		s << mat[i][col - 1];
		s << '\n';
	}
	return s;
}
#endif //__GEOMETRY_H__
