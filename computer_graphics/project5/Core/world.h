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
#include "light.h"
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

		unsigned int LightIndex;
		std::vector<Light*> light;

		pthread_mutex_t world_mutex;

	public:
		World();
		~World();

		//Polygon operation
		void PushPolygon();
		void PopPolygon();
		Polygon* GetPolygon();
		const std::vector<Polygon*>& GetPolygonStream();
		int GetPolygonCount();
		int GetPolygonIndex();
		void FlushPolygon();
		void UpdatePolygon();
		void SetPolygonIndex(unsigned int Index);
		void SavePolygon(const char* filename);
		void LoadPolygon(const char* filename);
		void SwapPolygon(unsigned int First, unsigned int Second);

		//Light operation
		void PushLight();
		void PopLight();
		Light* GetLight();
		const std::vector<Light*>& GetLightStream();
		int GetLightCount();
		int GetLightIndex();
		void FlushLight();
		void SetLightIndex(unsigned int Index);
		void SaveLight(const char* filename);
		void LoadLight(const char* filename);
		void SwapLight(unsigned int First, unsigned int Second);

		//World operation
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
