#ifndef _WORLD_DEFINE_H_
#define _WORLD_DEFINE_H_

#include <iostream>

using namespace std;

#ifdef __linux__

#include <sys/time.h>

#define INVALID_SOCKET -1
typedef int SOCKET;

#else

#include <time.h>

#endif

#define LEFT 	0
#define MIDDLE 	1
#define RIGHT 	2

#define SHIFT	256
#define CTRL	257
#define ALT		258

#define MAX_NAME_LENGTH 20
#define MAX_MSG_LENGTH 500
#define MAX_BUFFER_LENGTH 5000
#define MAX_SPHERE_COUNT 100
#define MAX_PLAYER_COUNT 10

#define RADIUS 0.05f
#define INIT_SPEED 0.4f
#define GRAVITY 0.04f
#define WALKING_SPEED 0.2f
#define SHOOT_POWER 1.0f

struct Sphere
{
	float position[3];
	float velocity[3];
	float color[3];
	float radius;
};

struct Cube
{
	float position[3];
	float color[3];
	float size;
};

struct Player
{
	char name[MAX_NAME_LENGTH];
	float position[3];
	float rotation[3];
	float velocity[3];
	float color[3];
	bool shot;
	int hp;
};

struct Word
{
	float position[2];
	float color[3];
	char content[100];
};

#endif
