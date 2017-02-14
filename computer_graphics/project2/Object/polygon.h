#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "../Core/world_define.h"
#include "../Core/pixel.h"

#include <fstream>
#include <cmath>

class Polygon
{
	private:
		std::vector<Vec3f> Vertex;
		std::vector<Vec2d> Edge;

		std::vector<Vec2f> ProjectionVertex;
		std::vector<Vec2f> XZProjectionVertex;
		std::vector<Vec2f> YZProjectionVertex;
		
		Vec2f MinProjectionCoor;
	   	Vec2f MaxProjectionCoor;

		Vec2f MinXZProjectionCoor;
		Vec2f MaxXZProjectionCoor;

		Vec2f MinYZProjectionCoor;
		Vec2f MaxYZProjectionCoor;

		Vec3f Center;

		Vec3d Color;

		Vec3f SpinPoint;
		Vec3f SpinDirection;
		
		float SpinRate;

		bool Raster;
		int Index;

	public:
		Polygon();
		~Polygon();

		void SetVertex(const std::vector<Vec3f>& NewVertex, const std::vector<Vec2d>& NewEdge);
		const std::vector<Vec3f>& GetVertex();
		const std::vector<Vec2d>& GetEdge();

		bool GetExtremeProjectionCoor(Vec2f& Min, Vec2f& Max);

		bool GetExtremeXZProjectionCoor(Vec2f& Min, Vec2f& Max);
		bool GetExtremeYZProjectionCoor(Vec2f& Min, Vec2f& Max);

		void Translate(const Vec3f& Offset);
		void Rotate(const Vec3f& Point, const Vec3f& Direction, float Angle);
		void Scale(const Vec3f& Factor);

		bool IsInside(const Vec2d& Point, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		void Intersect(const Vec2d& Point, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		void SutherlandHodgeman(const Vec2d& Min, const Vec2d& Max);

		void SetRasterize(bool State);
		bool GetRasterize();

		const Vec3d& GetColor();
		void SetColor(const Vec3d& NewColor);

		const Vec3f& GetCenter();

		void SetSpin(const Vec3f& Point, const Vec3f& Direction, float Rate);
		const Vec3f& GetSpinPoint();
		const Vec3f& GetSpinDirection();
		float 		 GetSpinRate();

		void SetIndex(int Num);

		void Update();
		void Draw();
};

#endif
