#include "mesh.h"

Mesh::Mesh()
{
	UpdateOnFrame = false;
	Collidable = false;
	Textured = false;
	Visible = true;
}

Mesh::~Mesh()
{
}

Vec3f Mesh::GetMeshColor()
{
	return Color;
}

void Mesh::SetMeshColor(const Vec3f& NewColor)
{
	Color = NewColor;
}

void Mesh::Draw()
{	
	if(!Visible)
		return;

	glPushMatrix();

	if(texture_handle != 0)
	{
		glEnable(GL_TEXTURE_2D);	
		glBindTexture(GL_TEXTURE_2D, texture_handle);
	}

	glTranslatef(Position.x, Position.y, Position.z);
	glRotatef(Rotation.x, 0.0f, 0.0f, 1.0f);
	glRotatef(Rotation.y, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotation.z, 0.0f, 1.0f, 0.0f);
	glColor3f(Color.x, Color.y, Color.z);
	glBegin(GL_TRIANGLE_STRIP);

	for(unsigned int i = 0; i < VertexIndices.size(); i++)
	{
		glNormal3f(Normals[NormalIndices[i]].x,
				   Normals[NormalIndices[i]].y,
				   Normals[NormalIndices[i]].z);

		if(Textured)
			glTexCoord2f(Textures[TextureIndices[i]].x, 
						 Textures[TextureIndices[i]].y);

		glVertex3f(Vertices[VertexIndices[i]].x,
				   Vertices[VertexIndices[i]].y,
				   Vertices[VertexIndices[i]].z);	
	}

	glEnd();

	if(texture_handle != 0)
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

bool Mesh::LoadModel(const char* Filename)
{
	FILE* file = fopen(Filename, "r");

	if(file == NULL)
		return false;

	while(1)
	{
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if(res == EOF)
			break;

		if(strcmp(lineHeader, "v") == 0)
		{
			Vec3f vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			Vertices.push_back(vertex);
		}
		else if(strcmp(lineHeader, "vt") == 0)
		{
			Vec2f uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			Textures.push_back(uv);
		}
		else if(strcmp(lineHeader, "vn") == 0)
		{
			Vec3f normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			Normals.push_back(normal);
		}
		else if(strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
					&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
					&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]); 

			if(matches != 9)
				return false;

			VertexIndices.push_back(vertexIndex[0] - 1);
			VertexIndices.push_back(vertexIndex[1] - 1);
			VertexIndices.push_back(vertexIndex[2] - 1);

			TextureIndices.push_back(uvIndex[0]);
			TextureIndices.push_back(uvIndex[1]);
			TextureIndices.push_back(uvIndex[2]);

			NormalIndices.push_back(normalIndex[0] - 1);
			NormalIndices.push_back(normalIndex[1] - 1);
			NormalIndices.push_back(normalIndex[2] - 1);
		}
	}

	return true;
}

bool Mesh::LoadTexture(const char* Filename)
{
	unsigned char info[54];
	unsigned char* data;

	FILE* file = fopen(Filename, "rb");

	if(file == NULL)
		return false;

	fread(info, sizeof(unsigned char), 54, file);

	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	data = new unsigned char[3 * width * height];

	fread(data, 3 * width * height, 1, file);

	fclose(file);

	//BGR to RGB
	unsigned char temp;
	for(int i = 0; i < width * height; i++)
	{
		temp = data[3 * i];
		data[3 * i] = data[3 * i + 2];
		data[3 * i + 2] = temp;
	}

	glGenTextures(1, &texture_handle);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	delete[] data;

	return true;
}

