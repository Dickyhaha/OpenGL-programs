#ifndef _PIXEL_H_
#define _PIXEL_H_

#include "world_define.h"
#include "pthread.h"

#include <list>
#include <cstring>
#include <cstdio>

class Pixel
{
	private:
		float* Buffer;

		Vec2d Size;
		Vec2d Position;

		Vec2d ClipMin;
		Vec2d ClipMax;

		Vec2f MaxWorldCoor;
		Vec2f MinWorldCoor;

		PROJECTION ProjectionPlane;

		bool Erase;

		pthread_mutex_t pixel_mutex;

	public:
		Pixel();
		~Pixel();

		void Lock();
		void Unlock();

		void SetErase(bool State);

		void SetExtremeWorldCoor(const Vec2f& Min, const Vec2f& Max);

		PROJECTION GetProjectionPlane();
		void SetProjectionPlane(PROJECTION Projection);

		Vec2d GetPixelPosition() const;
		void SetPixelPosition(const Vec2d& NewPosition);

		Vec2d GetPixelSize() const;
		void SetPixelSize(const Vec2d& NewSize);

		Vec3d GetPixelColor(const Vec2d& Position) const;
		bool SetPixelColor(const Vec2d& Position, const Vec3d& NewColor);

		void Swap(Vec2d& Start, Vec2d& End, int& dx, int& dy);

		void Horizontal(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color = WHITE);
		void Vertical(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color = WHITE);
		void Straight(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color = WHITE);
		void DDA(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color = WHITE);
		void Bresenham(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color = WHITE);
		void DrawLine(Vec2d Start, Vec2d End, const Vec3d& Color = WHITE, bool Algorithm = ALGO_BRESENHAM);

		void DrawCircle(const Vec2d& Center, int Radius, const Vec3d& Color = WHITE);
		void DrawVertex(const std::vector<Vec2f>& Vertex, const std::vector<Vec2d>& Edge, const Vec3d& Color = WHITE);
		void DrawNumber(const Vec2d& Position, int Value, const Vec3d& Color = WHITE);
		void DrawLetter(const Vec2d& Position, const char* String, const Vec3d& Color = WHITE);

		void Rasterize(const std::vector<Vec3f>& Vertex, const Vec3d& color);
	
		int Code(const Vec2d& Point);
		Vec2d Intersect(const Vec2d& Start, const Vec2d& End, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		bool CohenSutherland(Vec2d& Start, Vec2d& End);
		void Clip(const Vec2d& Min, const Vec2d& Max);
		void Unclip();

		void Clear();
		bool Init(const Vec2d& Size, bool EraseState = true, PROJECTION Projection = PROJECTION_XY);

		void Render();
};

extern Pixel canvas;
extern Pixel hub;

extern Pixel canvas1;
extern Pixel canvas2;

#endif
