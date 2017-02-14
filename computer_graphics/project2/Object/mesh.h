#ifndef _MESH_H_
#define _MESH_H_

#include <GL/freeglut.h>

#include <vector>

#include <cstdio>
#include <cstring>

#include "../Core/world_define.h"
#include "object.h"

class Mesh: public Object
{
	protected:
		std::vector<Vec2f> Textures;
		std::vector<Vec3f> Vertices;
		std::vector<Vec3f> Normals;
		
		std::vector<int> TextureIndices;
		std::vector<int> VertexIndices;
		std::vector<int> NormalIndices;

		Vec3f Color;	

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

		Vec3f GetMeshColor();
		void SetMeshColor(const Vec3f& NewColor);

		bool LoadModel(const char* Filename);
		bool LoadTexture(const char* Filename);
};

#endif
