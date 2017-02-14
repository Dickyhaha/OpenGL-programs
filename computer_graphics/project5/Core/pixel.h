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

		Vec2d DefaultSize;
		Vec2d Size;

		bool HalfTone;

		pthread_mutex_t pixel_mutex;

	public:
		Pixel();
		~Pixel();

		void Lock();
		void Unlock();

		Vec2d GetDefaultSize();
		void SetDefaultSize(const Vec2d& NewDefaultSize);

		Vec2d GetPixelSize() const;
		void SetPixelSize(const Vec2d& NewSize);

		Vec3f GetPixelColor(const Vec2d& Position) const;
		bool SetPixelColor(const Vec2d& Position, const Vec3f& Color);

		//Line drawing
		void Swap(Vec2d& Start, Vec2d& End, Vec3f& StartColor, Vec3f& EndColor, int& dx, int& dy);

		void Horizontal(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor = WHITE, const Vec3f& EndColor = WHITE);
		void Vertical(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor = WHITE, const Vec3f& EndColor = WHITE);
		void Straight(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor = WHITE, const Vec3f& EndColor = WHITE);
		void DDA(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor = WHITE, const Vec3f& EndColor = WHITE);
		void Bresenham(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor = WHITE, const Vec3f& EndColor = WHITE);
		void DrawLine(Vec2d Start, Vec2d End, Vec3f StartColor = WHITE, Vec3f EndColor = WHITE, bool Algorithm = ALGO_BRESENHAM);

		//Complex primitive drawing
		void DrawCircle(const Vec2d& Center, int Radius, const Vec3f& Color = WHITE);
		void DrawNumber(const Vec2d& Position, int Value, const Vec3f& Color = WHITE);
		void DrawLetter(const Vec2d& Position, const char* String, const Vec3f& Color = WHITE);
		void DrawVertex(const std::vector<Vec2f>& Vertex, const std::vector<Vec3d>& Face, const std::vector<bool>& FrontFace, const std::vector< std::vector<Vec3f> >& Color, const std::vector<int>& FaceColor, const std::vector<bool>& Sight);

		Vec2d Interpolate(Vec2d Start, Vec2d End, int y);
		Vec3f InterpolateColor(Vec2d Start, Vec2d End, Vec3f StartColor, Vec3f EndColor,  int y);
		void Rasterize(const std::vector<Vec2f>& Vertex, const std::vector<Vec3d>& Face, const std::vector<bool>& FrontFace, const std::vector< std::vector<Vec3f> >& Color, const std::vector<int>& FaceColor, const std::vector<bool>& Sight);

		//Clipping
		int Code(const Vec2d& Point);
		Vec2d Intersect(const Vec2d& Start, const Vec2d& End, const Vec2d& Min, const Vec2d& Max, int CheckCode);
		bool CohenSutherland(Vec2d& Start, Vec2d& End);
		void Clip(const Vec2d& Min, const Vec2d& Max);
		void Unclip();

		//Half Toning
		void SetHalfTone(bool HalfToneState);

		void Clear();

		bool Init(const Vec2d& Size, bool HalfTone = false);

		void Render();
};

extern Pixel hub;
#endif
