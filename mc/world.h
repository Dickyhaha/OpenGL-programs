#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <pthread.h>

#include "world_define.h"
#include "mesh.h"
#include "terrain.h"
#include "character.h"

using namespace std;

class World
{
	private:
		//--Client only--//
		int key[259];
		int button[3];

		bool updated;

		float ambient_color[4];
		float diffuse_color[4];
		float diffuse_position[4];

		Terrain terrain;

		Character character;
		std::vector<Character> enemy;

		pthread_mutex_t world_mutex;

	public:
		World();
		~World();

		void UpdateMouse(int index, int state);
		void UpdateKey(int index, int state);
		void UpdateView(float rx, float ry);

		bool IsUpdate();
		void Update();	
		void Draw();
		bool Init();

		void Lock();
		void Unlock();
		
		float* GetAmbientColor();
		float* GetDiffuseColor();
		float* GetDiffusePosition();

		Vec3 GetRotation();
		Vec3 GetPosition();
		
};

#endif
