#include "terrain.h"

Terrain::Terrain()
{
	HeightMap = NULL;
}

Terrain::~Terrain()
{
	if(HeightMap != NULL)
	{
		for(int i = 0; i < maxX - minX + 1; i++)
			delete[] HeightMap[i];
		delete[] HeightMap;
		HeightMap = NULL;
	}
}

bool Terrain::LoadTerrain(const char* Filename)
{
	if(!LoadModel(Filename))
		return false;

	minX = Vertices[0].x;
	maxX = Vertices[0].x;
	minZ = Vertices[0].z;
	maxZ = Vertices[0].z;
	for(unsigned int i = 1; i < Vertices.size(); i++)
	{
		if(Vertices[i].x < minX)
			minX = Vertices[i].x;
		if(Vertices[i].z < minZ)
			minZ = Vertices[i].z;

		if(Vertices[i].x > maxX)
			maxX = Vertices[i].x;
		if(Vertices[i].z > maxZ)
			maxZ = Vertices[i].z;
	}

	HeightMap = new float*[maxX - minX + 1];
	for(int i = 0; i < maxX - minX + 1; i++)
		HeightMap[i] = new float[maxZ - minZ + 1];

	for(unsigned int i = 0; i < Vertices.size(); i++)
		HeightMap[(int)Vertices[i].x - minX][(int)Vertices[i].z - minZ] = Vertices[i].y;

	return true;
}

float Terrain::GetTerrainHeight(Vec3 Position)
{
	if(Position.x < minX + 1)
		Position.x = minX + 1;
	else if(Position.x > maxX - 1)
		Position.x = maxX - 1;

	if(Position.z < minZ + 1)
		Position.z = minZ + 1;
	else if(Position.z > maxZ - 1)
		Position.z = maxZ - 1;

	float h[4] = 
	{
		HeightMap[(int)Position.x - minX][(int)Position.z - minZ],
		HeightMap[(int)Position.x - minX + 1][(int)Position.z - minZ],
		HeightMap[(int)Position.x - minX + 1][(int)Position.z - minZ + 1],
		HeightMap[(int)Position.x - minX][(int)Position.z - minZ + 1]
	};

	float h1 = (h[1] - h[0]) * fmod(Position.x, 1.0f) + h[0];
	float h2 = (h[2] - h[3]) * fmod(Position.x, 1.0f) + h[3];

	return (h2 - h1) * fmod(Position.z, 1.0f) + h1;
}
