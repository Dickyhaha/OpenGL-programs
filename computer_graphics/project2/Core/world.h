#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <pthread.h>

#include <fstream>

#include "world_define.h"
#include "../Object/mesh.h"
#include "../Object/character.h"
#include "../Object/polygon.h"

using namespace std;

class World
{
	private:
		//--Client only--//
		int key[259];
		int button[3];

		bool updated;

		unsigned int PolygonIndex;
		std::vector<Polygon*> polygon;

		pthread_mutex_t world_mutex;

	public:
		World();
		~World();

		void PushPolygon();
		void PopPolygon();
		Polygon* GetPolygon();
		int GetPolygonCount();
		int GetPolygonIndex();
		void FlushPolygon();
		void UpdatePolygon();
		void SetPolygonIndex(unsigned int Index);
		void SavePolygon(const char* filename);
		void LoadPolygon(const char* filename);
		void Swap(unsigned int First, unsigned int Second);

		//End of polygon

		void UpdateMouse(int index, int state);
		void UpdateKey(int index, int state);
		void UpdateView(float rx, float ry);

		bool IsUpdate();
		void Update();	
		void Draw();
		bool Init();

		void Lock();
		void Unlock();		
};

extern World world;

#endif
