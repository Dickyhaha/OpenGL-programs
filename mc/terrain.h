#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "mesh.h"

class Terrain: public Mesh
{
	private:
		int minX, minZ, maxX, maxZ;
		float ** HeightMap;

	public:
		Terrain();
		~Terrain();

		bool LoadTerrain(const char* Filename);
		float GetTerrainHeight(Vec3 Position);
};

#endif
