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

		bool Erase;

		pthread_mutex_t pixel_mutex;

	public:
		Pixel();
		~Pixel();

		void Lock();
		void Unlock();

		void SetErase(bool State);

		Vec2d GetPixelPosition() const;
		void SetPixelPosition(const Vec2d& NewPosition);

		Vec2d GetPixelSize() const;
		void SetPixelSize(const Vec2d& NewSize);

		Vec3f GetPixelColor(const Vec2d& Position) const;
		bool SetPixelColor(const Vec2d& Position, const Vec3f& NewColor);

		void Swap(Vec2d& Start, Vec2d& End, int& dx, int& dy);

		void Horizontal(const Vec2d& Start, const Vec2d& End, int dx, int dy);
		void Vertical(const Vec2d& Start, const Vec2d& End, int dx, int dy);
		void Straight(const Vec2d& Start, const Vec2d& End, int dx, int dy);
		void DDA(const Vec2d& Start, const Vec2d& End, int dx, int dy);
		void Bresenham(const Vec2d& Start, const Vec2d& End, int dx, int dy);
		void DrawLine(Vec2d Start, Vec2d End, bool Algorithm = ALGO_BRESENHAM);

		void DrawCircle(const Vec2d& Center, int Radius);
		void DrawPolygon(const std::vector<Vec2f>& Vertex);
		void DrawNumber(const Vec2d& Position, int Value);

		void Rasterize(const std::vector<Vec2f>& Vertex);
	
		int Code(const Vec2d& Point);
		Vec2d Intersect(const Vec2d& Start, const Vec2d& End, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		bool CohenSutherland(Vec2d& Start, Vec2d& End);
		void Clip(const Vec2d& Min, const Vec2d& Max);
		void Unclip();

		void Clear();
		bool Init(const Vec2d& Size, bool EraseState = false);

		void Draw(Pixel& Other);
		void Render();
};

extern Pixel canvas;
extern Pixel hub;

#endif
