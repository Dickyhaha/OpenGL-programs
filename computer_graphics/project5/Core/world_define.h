#ifndef _WORLD_DEFINE_H_
#define _WORLD_DEFINE_H_

#include <GL/freeglut.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <cmath>

#include <sys/time.h>
#include <pthread.h>

#define MOUSEDOWN	0
#define MOUSEUP  	1
#define MOUSEHOLD	2
#define MOUSEHOVER	3

#define ALGO_DDA		0
#define ALGO_BRESENHAM	1

#define IN		0b0000
#define LEFT	0b0001
#define UP		0b0010
#define RIGHT	0b0100
#define DOWN 	0b1000

#define WHITE 	Vec3f(1.0f, 1.0f, 1.0f)
#define BLACK 	Vec3f(0.0f, 0.0f, 0.0f)
#define RED 	Vec3f(1.0f, 0.0f, 0.0f)
#define GREEN 	Vec3f(0.0f, 1.0f, 0.0f)
#define BLUE 	Vec3f(0.0f, 0.0f, 1.0f)
#define CYAN	Vec3f(0.0f, 1.0f, 1.0f)

#define PROJECTION_XY 0
#define PROJECTION_XZ 1
#define PROJECTION_YZ 2

#define AMBIENT 0
#define DIFFUSE 1

using namespace std;

typedef bool 	VIEW;
typedef int		PROJECTION;
typedef bool	LIGHT;

template <typename T>
struct Vec2
{
	T x, y;

	Vec2()
	{ x = 0.0f; y = 0.0f; }

	template <typename S>
	Vec2(const S x1, const S y1)
	{ x = x1; y = y1; }

	template <typename S>
	Vec2(const S pos[2])
	{ x = pos[0]; y = pos[1]; }

	template <typename S>
	Vec2(const Vec2<S>& other)
	{ x = other.x; y = other.y; }

	template <typename S>
	void operator=(const Vec2<S>& other)
	{ x = other.x; y = other.y; }

	template <typename S>
	bool operator==(const Vec2<S>& other) const
	{ return x == other.x && y == other.y; }

	template <typename S>
	bool operator!=(const Vec2<S>& other) const
	{ return !operator==(other); }

	template <typename S>
	bool operator<(const Vec2<S>& other) const
	{ return x < other.x || y < other.y; }

	template <typename S>
	bool operator>=(const Vec2<S>& other) const
	{ return x >= other.x || y >= other.y; }

	template <typename S>
	Vec2<T> operator*(const Vec2<S>& other) const
	{ return Vec2<T>(x * other.x, y * other.y); }

	template <typename S>
	Vec2<T> operator/(const Vec2<S>& other) const
	{ return Vec2<T>(x / other.x, y / other.y); }

	template <typename S>
	Vec2<T> operator*(S val) const
	{ return Vec2<T>(x * val, y * val); }

	template <typename S>
	Vec2<T> operator/(S val) const
	{ return Vec2<T>(x / val, y / val); }

	template <typename S>
	Vec2<T> operator+(const Vec2<S>& other) const
	{ return Vec2<T>(x + other.x, y + other.y); }

	template <typename S>
	Vec2<T> operator-(const Vec2<S>& other) const
	{ return Vec2<T>(x - other.x, y - other.y); }

	template <typename S>
	Vec2<T> operator+(S val) const
	{ return Vec2<T>(x + val, y + val); }

	template <typename S>
	Vec2<T> operator-(S val) const
	{ return Vec2<T>(x - val, y - val); }

	void swap(Vec2<T>& other)
	{ Vec2<T> temp = other; other = *this; *this = temp; }

	T distance()
	{ return sqrt(x * x + y * y); }

	Vec2<int> round()
	{ return Vec2<int>(::round(x), ::round(y)); }
};

template <typename T>
struct Vec3
{
	T x, y, z;

	Vec3()
	{ x = 0.0f; y = 0.0f; z = 0.0f;	}

	template <typename S>
	Vec3(const S x1, const S y1, const S z1)
	{ x = x1; y = y1; z = z1; }

	template <typename S>
	Vec3(const S pos[3])
	{ x = pos[0]; y = pos[1]; z = pos[2]; }

	template <typename S>
	Vec3(const Vec2<S>& other)
	{ x = other.x; y = other.y; }

	template <typename S>
	void operator=(const Vec3<S>& other)
	{ x = other.x; y = other.y; z = other.z; }

	template <typename S>
	bool operator==(const Vec3<S>& other) const
	{ return x == other.x && y == other.y && z == other.z; }

	template <typename S>
	bool operator!=(const Vec3<S>& other) const
	{ return !operator==(other); }

	template <typename S>
	bool operator<(const Vec3<S>& other) const
	{ return x < other.x || y < other.y || z < other.z; }

	template <typename S>
	bool operator>(const Vec3<S>& other) const
	{ return x > other.x || y > other.y || z > other.z; }

	template <typename S>
	Vec3<T> operator+(const Vec3<S>& other) const
	{ return Vec3<T>(x + other.x, y + other.y, z + other.z); }

	template <typename S>
	Vec3<T> operator-(const Vec3<S>& other) const
	{ return Vec3<T>(x - other.x, y - other.y, z - other.z); }

	template <typename S>
	Vec3<T> operator+(S val) const
	{ return Vec3<T>(x + val, y + val, z + val); }

	template <typename S>
	Vec3<T> operator-(S val) const
	{ return Vec3<T>(x - val, y - val, z - val); }

	template <typename S>
	Vec3<T> operator*(const Vec3<S>& other) const
	{ return Vec3<T>(x * other.x, y * other.y, z * other.z); }

	template <typename S>
	Vec3<T> operator/(const Vec3<S>& other) const
	{ return Vec3<T>(x / other.x, y * other.y, z * other.z); }

	template <typename S>
	Vec3<T> operator*(const S val) const
	{ return Vec3<T>(x * val, y * val, z * val); }

	template <typename S>
	Vec3<T> operator/(const S val) const
	{ return Vec3<T>(x / val, y / val, z / val); }

	template <typename S>
	void operator+=(const Vec3<S>& other)
	{ x += other.x; y += other.y; z += other.z; }

	template <typename S>
	void operator-=(const Vec3<S>& other)
	{ x -= other.x; y -= other.y; z -= other.z; }

	template <typename S>
	void operator*=(const Vec3<S>& other)
	{ x *= other.x; y *= other.y; z *= other.z; }

	template <typename S>
	void operator/=(const Vec3<S>& other)
	{ x /= other.x; y /= other.y; z /= other.z; }

	Vec3<T> normalize() const
	{ T d = distance(); return Vec3<T>(x / d, y / d, z / d);}

	T sum() const
	{ return x + y + z; }	

	T distance() const
	{ return sqrt(x * x + y * y + z * z); }

	template <typename S>
	Vec3<T> cross(const Vec3<S>& other) const
	{ return Vec3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }

	void swap(Vec3<T>& other)
	{ Vec3<T> temp = other; other = *this; *this = temp; }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T> v)
{ os << v.x << " " << v.y << endl; return os; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec3<T> v)
{ os << v.x << " " << v.y << " " << v.z << endl; return os; }

typedef Vec3<int>	Vec3d;
typedef Vec3<float> Vec3f;

typedef Vec2<int> 	Vec2d;
typedef Vec2<float> Vec2f;

const int Character[3][7][5] = {

	//X
	{{1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 0, 1, 0},
	 {0, 0, 1, 0, 0},
	 {0, 1, 0, 1, 0},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1}},
	
	//Y
	{{1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1 ,0, 1, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0}},

	//Z
	{{1, 1, 1, 1, 1},
	 {0, 0, 0, 0, 1},
	 {0, 0, 0, 1, 0},
	 {0, 0, 1, 0, 0},
	 {0, 1, 0, 0, 0},
	 {1, 0, 0, 0, 0},
	 {1, 1, 1, 1, 1}}
};

const int Digit[10][7][5] = {

	{{0, 1, 1, 1, 0},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 1, 1, 0}},

	{{0, 0, 1, 0, 0},
	 {0, 1, 1, 0, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0},
	 {0, 1, 1, 1, 0}},

	{{0, 1, 1, 1, 0},
	 {1, 0, 0, 0, 1},
	 {0, 0, 0, 0, 1},
	 {0, 1, 1, 1, 0},
	 {1, 0, 0, 0, 0},
	 {1, 0, 0, 0, 0},
	 {1, 1, 1, 1, 1}},

	{{0, 1, 1, 1, 0},
	 {1, 0, 0, 0, 1},
	 {0, 0, 0, 0, 1},
	 {0, 0, 1 ,1, 0},
	 {0, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 1, 1, 0}},

	{{0, 0, 0, 1, 0},
	 {0, 0, 1, 1, 0},
	 {0, 1, 0, 1, 0},
	 {1, 0, 0, 1, 0},
	 {1, 1, 1, 1, 1},
	 {0, 0, 0, 1, 0},
	 {0, 0, 0, 1, 0}},

	{{1, 1, 1, 1, 1},
	 {1, 0, 0, 0, 0},
	 {1, 0, 0, 0, 0},
	 {1, 1, 1, 1, 0},
	 {0, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 1, 1, 0}},

	{{0, 1, 1, 1, 0},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 0},
	 {1, 1, 1, 1, 0},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 1, 1, 0}},

	{{1, 1, 1, 1, 1},
	 {0, 0, 0, 0, 1},
	 {0, 0, 0, 1, 0},
	 {0, 0, 1, 0, 0},
	 {0, 1 ,0, 0, 0},
	 {0, 1, 0, 0, 0},
	 {0, 1, 0, 0, 0}},

	{{0, 1, 1, 1, 0},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 1, 1, 0},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 1, 1, 0}},

	{{0, 1, 1, 1, 0},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 1, 1, 1},
	 {0, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 1, 1, 0}}
};

#endif
