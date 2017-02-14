#ifndef _MESH_H_
#define _MESH_H_

#include <GL/freeglut.h>

#include <vector>

#include <cstdio>
#include <cstring>

#include "world_define.h"
#include "object.h"

class Mesh: public Object
{
	protected:
		std::vector<Vec2> Textures;
		std::vector<Vec3> Vertices;
		std::vector<Vec3> Normals;
		
		std::vector<int> TextureIndices;
		std::vector<int> VertexIndices;
		std::vector<int> NormalIndices;

		Vec3 Color;	

		GLuint texture_handle;
		
	//Public Attributes
	public:
		bool UpdateOnFrame;
		bool Collidable;
		bool Textured;
		bool Visible;

	public:
		Mesh();
		~Mesh();

		void Draw();

		Vec3 GetMeshColor();
		void SetMeshColor(const Vec3& NewColor);

		bool LoadModel(const char* Filename);
		bool LoadTexture(const char* Filename);
};

#endif
