#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <fstream>
#include <iostream>
#include <exception>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class t> struct Vec2 {
	t raw[2];
	t& x;
	t& y;
	Vec2<t>() : raw(), x(raw[0]), y(raw[1]) { x = y = t(); }
	Vec2<t>(t _x, t _y) : raw(), x(raw[0]), y(raw[1]) { x = _x; y = _y; }
	Vec2<t>(const Vec2<t> &v) : raw(), x(raw[0]), y(raw[1]) { *this = v; }
	Vec2<t> & operator =(const Vec2<t> &v) {
		if (this != &v) {
			x = v.x;
			y = v.y;
		}
		return *this;
	}
	Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(x + V.x, y + V.y); }
	Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(x - V.x, y - V.y); }
	Vec2<t> operator *(float f)          const { return Vec2<t>(x*f, y*f); }
	t       operator *(const Vec2<t> &v) const { return x * v.x + y * v.y; }
	t& operator[](const int i) { return raw[i]; }


	t Dot(const Vec2<t> &V0) const 
	{
		return x * V0.x + y * V0.y;
	}
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
	t x, y, z;
	Vec3<t>() : x(t()), y(t()), z(t()) { }
	Vec3<t>(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
	//Vec3<t>(Matrix m);
	template <class u> Vec3<t>(const Vec3<u> &v);
	Vec3<t> operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z - z * v.y, z*v.x - x * v.z, x*v.y - y * v.x); }
	Vec3<t> operator +(const Vec3<t> &v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
	Vec3<t> operator -(const Vec3<t> &v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
	Vec3<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
	t       operator *(const Vec3<t> &v) const { return x * v.x + y * v.y + z * v.z; }
	float norm() const { return std::sqrt(x*x + y * y + z * z); }
	Vec3<t> & normalize(t l = 1) { *this = (*this)*(l / norm()); return *this; }
	t& operator[](const int i) { return i <= 0 ? x : (1 == i ? y : z); }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);



	//template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

template <typename T> Vec3<T> cross(Vec3<T> v1, Vec3<T> v2) {
	return Vec3<T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template <class t> struct Vec4 {
	t x, y, z, w;
	Vec4<t>() : x(t()), y(t()), z(t()), w(t()) { }
	Vec4<t>(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}

	Vec4<t> operator +(const Vec4<t> &v) const { return Vec4<t>(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vec4<t> operator -(const Vec4<t> &v) const { return Vec4<t>(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vec4<t> operator *(float f)          const { return Vec4<t>(x*f, y*f, z*f, w*f); }
	t       operator *(const Vec4<t> &v) const { return x * v.x + y * v.y + z * v.z + w*v.w; }
	t& operator[](const int i) { return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w)); }

	template <class > friend std::ostream& operator<<(std::ostream& s, Vec4<t>& v);
};



typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;
typedef Vec4<float> Vec4f;

template <> template <> Vec3<int>::Vec3(const Vec3<float> &v);
template <> template <> Vec3<float>::Vec3(const Vec3<int> &v);

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec4<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
	return s;
}



#endif //__GEOMETRY_H__
