#include "polygon.h"

Polygon::Polygon(): DiffuseColor(WHITE), SpecularColor(BLACK), SpinRate(0.0f), Index(-1)
{
}

Polygon::~Polygon()
{
}

const std::vector<Vec3f>& Polygon::GetVertex()
{
	return Vertex;
}

const std::vector<Vec3f>& Polygon::GetNormal()
{
	return Normal;
}

const std::vector<Vec3d>& Polygon::GetFace()
{
	return Face;
}

const std::vector<Vec3d>& Polygon::GetFaceNormal()
{
	return FaceNormal;
}

const char* Polygon::GetOBJ()
{
	return FileName;
}

bool Polygon::LoadModel(const char* Filename)
{
	char Obj[100] = "obj_files/";
	strcat(Obj, Filename);
	strcat(Obj, ".obj");

	FILE* file = fopen(Obj, "r");

	if(file == NULL)
		return false;

	Vertex.clear();
	Normal.clear();
	Face.clear();
	FaceNormal.clear();

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
			Vertex.push_back(vertex);
		}
		else if(strcmp(lineHeader, "vn") == 0)
		{
			Vec3f normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			Normal.push_back(normal);
		}
		else if(strcmp(lineHeader, "f") == 0)
		{
			Vec3d vertexIndex, uvIndex, normalIndex;
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
					&vertexIndex.x, &uvIndex.x, &normalIndex.x, 
					&vertexIndex.y, &uvIndex.y, &normalIndex.y, 
					&vertexIndex.z, &uvIndex.z, &normalIndex.z); 

			if(matches != 9)
				return false;

			Face.push_back(vertexIndex - 1);
			FaceNormal.push_back(normalIndex - 1);
		}
	}

	fclose(file);

	strcpy(FileName, Filename);

	return true;
}

//Polygon translation
void Polygon::Translate(const Vec3f& Offset)
{
	if(Offset == Vec3f())
		return;

	for(unsigned int i = 0; i < Vertex.size(); i++)
		Vertex[i] = Vertex[i] + Offset;
}

//Polygon rotation
void Polygon::Rotate(const Vec3f& Point, const Vec3f& Direction, float Angle)
{
	if(Point == Direction)
		return;

	Vec3f v = Direction - Point;

	float l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	Vec3f u = Vec3f(v.x / l, v.y /l, v.z / l);
	Vec3f temp;

	float sinA = sin(Angle);
	float cosA = cos(Angle);

	for(unsigned int i = 0; i < Vertex.size(); i++)
	{
		v = Vertex[i] - Point;

		temp.x = (cosA + u.x * u.x * (1 - cosA)) 		* v.x + 
			(u.x * u.y * (1 - cosA) - u.z * sinA) 		* v.y + 
			(u.x * u.z * (1 - cosA) + u.y * sinA) 		* v.z;

		temp.y = (u.y * u.x * (1 - cosA) + u.z * sinA) 	* v.x +
			(cosA + u.y * u.y * (1 - cosA)) 			* v.y + 
			(u.y * u.z * (1 - cosA) - u.x * sinA) 		* v.z;

		temp.z = (u.z * u.x * (1 - cosA) - u.y * sinA)	* v.x +
			(u.z * u.y * (1 - cosA) + u.x * sinA)		* v.y +
			(cosA + u.z * u.z * (1 - cosA))				* v.z;

		Vertex[i] = temp + Point;
	}

	for(unsigned int i = 0; i < Normal.size(); i++)
	{
		v = Normal[i];
		
		temp.x = (cosA + u.x * u.x * (1 - cosA)) 		* v.x + 
			(u.x * u.y * (1 - cosA) - u.z * sinA) 		* v.y + 
			(u.x * u.z * (1 - cosA) + u.y * sinA) 		* v.z;

		temp.y = (u.y * u.x * (1 - cosA) + u.z * sinA) 	* v.x +
			(cosA + u.y * u.y * (1 - cosA)) 			* v.y + 
			(u.y * u.z * (1 - cosA) - u.x * sinA) 		* v.z;

		temp.z = (u.z * u.x * (1 - cosA) - u.y * sinA)	* v.x +
			(u.z * u.y * (1 - cosA) + u.x * sinA)		* v.y +
			(cosA + u.z * u.z * (1 - cosA))				* v.z;

		Normal[i] = temp;
	}
}

//Polygon scaling
void Polygon::Scale(const Vec3f& Factor)
{
	if(Factor.x <= 0.0f || Factor.y <= 0.0f || Factor.z <= 0.0f)
		return;

	for(unsigned int i = 0; i < Vertex.size(); i++)
		Vertex[i] = (Vertex[i] - Center) * Factor + Center;
}

bool Polygon::IsInside(const Vec2d& Point, const Vec2d& Min, const Vec2d& Max, int CheckCode)
{
	if(CheckCode & UP)
		return Point.y <= Max.y;
	if(CheckCode & DOWN)
		return Point.y >= Min.y;
	if(CheckCode & LEFT)
		return Point.x >= Min.x;
	if(CheckCode & RIGHT)
		return Point.x <= Max.x;

	return true;
}

//Unchanged, still in the 2d setting
void Polygon::SutherlandHodgeman(const Vec2d& Min, const Vec2d& Max)
{/*
	std::vector<Vec2d> Out = Vertex, In;

	for(unsigned int code = LEFT; code <= DOWN; code <<= 1)
	{
	In = Out;
	Out.clear();
	for(unsigned int i = 0; i < In.size(); i++)
	{
	Vec2d v1 = In[i];
	Vec2d v2 = In[(i + 1) % In.size()];

	bool code1 = IsInside(v1, Min, Max, code), code2 = IsInside(v2, Min, Max, code);

	if(!code1 && code2)
	{
	Out.push_back(
	Out.push_back(v2);
	}
	else if(code1 && code2)
	Out.push_back(v2);
	else if(code1 && !code2)
	Out.push_back(
	}
	}

	Vertex = Out;
	Update();*/
}

const Vec3f& Polygon::GetDiffuseColor()
{
	return DiffuseColor;
}

void Polygon::SetDiffuseColor(const Vec3f& NewDiffuseColor)
{
	DiffuseColor = NewDiffuseColor;
}

const Vec3f& Polygon::GetSpecularColor()
{
	return SpecularColor;
}

void Polygon::SetSpecularColor(const Vec3f& NewSpecularColor)
{
	SpecularColor = NewSpecularColor;
}

const Vec3f& Polygon::GetCenter()
{
	return Center;
}

void Polygon::SetSpin(const Vec3f& Point, const Vec3f& Direction, float Rate)
{
	SpinPoint = Point;
	SpinDirection = Direction;
	SpinRate = Rate;
}

const Vec3f& Polygon::GetSpinPoint()
{
	return SpinPoint;
}

const Vec3f& Polygon::GetSpinDirection()
{
	return SpinDirection;
}

float Polygon::GetSpinRate()
{
	return SpinRate;
}

void Polygon::SetIndex(int Num)
{
	Index = Num;
	Update();
}

//Update center of polygon and all projection coordinates
void Polygon::Update()
{
	if(Vertex.size() == 0)
		return;

	if(SpinRate > 0.0f)
		Rotate(SpinPoint, SpinDirection, SpinRate);

	Vec3f Mean = Vertex[0];

	for(unsigned int i = 1; i < Vertex.size(); i++)
		Mean = Mean +  Vertex[i];

	Center = Vec3f(Mean.x / Vertex.size(), Mean.y / Vertex.size(), Mean.z / Vertex.size());	
}

