#ifndef _WORLD_DEFINE_H_
#define _WORLD_DEFINE_H_

#include <GL/freeglut.h>

#include <iostream>
#include <vector>
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

#define WHITE 	Vec3d(255, 255, 255)
#define BLACK 	Vec3d(0  , 0  , 0  )
#define RED 	Vec3d(255, 0  , 0  )
#define GREEN 	Vec3d(0  , 255, 0  )
#define BLUE 	Vec3d(0  , 0  , 255)

#define PROJECTION_XY 0
#define PROJECTION_XZ 1
#define PROJECTION_YZ 2

using namespace std;

typedef bool 	VIEW;
typedef int		PROJECTION;

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
	Vec2<T> operator*(const Vec2<S>& other) const
	{ return Vec2<T>(x * other.x, y * other.y); }

	template <typename S>
	Vec2<T> operator/(const Vec2<S>& other) const
	{ return Vec2<T>(x / other.x, y / other.y); }

	template <typename S>
	Vec2<T> operator+(const Vec2<S>& other) const
	{ return Vec2<T>(x + other.x, y + other.y); }

	template <typename S>
	Vec2<T> operator-(const Vec2<S>& other) const
	{ return Vec2<T>(x - other.x, y - other.y); }
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
	Vec3<T> operator*(const Vec3<S>& other) const
	{ return Vec3<T>(x * other.x, y * other.y, z * other.z); }

	template <typename S>
	Vec3<T> operator/(const Vec3<S>& other) const
	{ return Vec3<T>(x / other.x, y * other.y, z * other.z); }
};


typedef Vec3<int>	Vec3d;
typedef Vec3<float> Vec3f;

typedef Vec2<int> 	Vec2d;
typedef Vec2<float> Vec2f;

const int Character[3][7][5] = {

	{{1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1, 0, 1, 0},
	 {0, 0, 1, 0, 0},
	 {0, 1, 0, 1, 0},
	 {1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1}},

	{{1, 0, 0, 0, 1},
	 {1, 0, 0, 0, 1},
	 {0, 1 ,0, 1, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0},
	 {0, 0, 1, 0, 0}},

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
