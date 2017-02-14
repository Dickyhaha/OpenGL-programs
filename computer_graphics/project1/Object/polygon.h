#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "../Core/world_define.h"
#include "../Core/pixel.h"

#include <fstream>
#include <cmath>

class Polygon
{
	private:
		std::vector<Vec2f> Vertex;
		Vec2f Center;

		Pixel pixel;
		bool Raster;

		float Spin;

		int Index;

	public:
		Polygon();
		~Polygon();

		void SetVertex(const std::vector<Vec2f>& NewVertex);
		const std::vector<Vec2f>& GetVertex();

		void Translate(const Vec2d& Offset);
		void Rotate(float Angle);
		void Scale(float Factorx, float Factory);

		bool IsInside(const Vec2d& Point, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		void Intersect(const Vec2d& Point, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		void SutherlandHodgeman(const Vec2d& Min, const Vec2d& Max);

		void SetRasterize(bool State);
		bool GetRasterize();

		void Spinning(float Rate);
		float GetSpinning();
		void SetIndex(int Num);

		void Update();
		void Draw();
};

#endif
