#ifndef _WORLD_DEFINE_H_
#define _WORLD_DEFINE_H_

#include <glm/glm.hpp>
#include <GL/freeglut.h>

#include <iostream>

#include <vector>

#include <cmath>

#include <sys/time.h>

#define LEFT 	0
#define MIDDLE 	1
#define RIGHT 	2

#define FIRST_PERSON 0
#define THIRD_PERSON 1

typedef bool Perspective;

struct Vec3
{
	float x;
	float y;
	float z;

	Vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vec3(const float x1, const float y1, const float z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}

	Vec3(const float pos[3])
	{
		x = pos[0];
		y = pos[1];
		z = pos[2];
	}
};

struct Vec2
{
	float x;
	float y;

	Vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vec2(const float x1, const float y1)
	{
		x = x1;
		y = y1;
	}

	Vec2(const float pos[3])
	{
		x = pos[0];
		y = pos[1];
	}
};

#endif
