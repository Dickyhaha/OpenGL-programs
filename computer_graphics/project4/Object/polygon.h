#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "../Core/world_define.h"
#include "../Core/pixel.h"

#include <fstream>
#include <cmath>

class Polygon
{
	private:
		//Values
		std::vector<Vec3f> Vertex;
		std::vector<Vec3f> Normal;
		//Indices
		std::vector<Vec3d> Face;
		std::vector<Vec3d> FaceNormal;

		Vec3f Center;

		Vec3f DiffuseColor;
		Vec3f SpecularColor;

		Vec3f SpinPoint;
		Vec3f SpinDirection;
		
		float SpinRate;

		int Index;

		char FileName[100];

	public:
		Polygon();
		~Polygon();

		const std::vector<Vec3f>& GetVertex();
		const std::vector<Vec3f>& GetNormal();
		const std::vector<Vec3d>& GetFace();
		const std::vector<Vec3d>& GetFaceNormal();

		const char* GetOBJ();
		bool LoadModel(const char* Filename);

		void Translate(const Vec3f& Offset);
		void Rotate(const Vec3f& Point, const Vec3f& Direction, float Angle);
		void Scale(const Vec3f& Factor);

		bool IsInside(const Vec2d& Point, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		void Intersect(const Vec2d& Point, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		void SutherlandHodgeman(const Vec2d& Min, const Vec2d& Max);

		const Vec3f& GetDiffuseColor();
		void SetDiffuseColor(const Vec3f& NewDiffuseColor);

		const Vec3f& GetSpecularColor();
		void SetSpecularColor(const Vec3f& NewSpecularColor);

		const Vec3f& GetCenter();

		void SetSpin(const Vec3f& Point, const Vec3f& Direction, float Rate);
		const Vec3f& GetSpinPoint();
		const Vec3f& GetSpinDirection();
		float 		 GetSpinRate();

		void SetIndex(int Num);

		void Update();
};

#endif
