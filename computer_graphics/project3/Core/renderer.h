#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "world_define.h"
#include "pixel.h"
#include "light.h"
#include "../Object/polygon.h"

class Renderer
{
	private:
		Pixel Canvas;

		PROJECTION Projection;

		Vec3f FromVector;
		Vec3f AtVector;
		Vec3f UpVector;

		float ViewAngle;

		float Zmin;
		float Zmax;

	public:
		Renderer();
		~Renderer();

		Vec3f GetFromVector();
		void SetFromVector(const Vec3f& NewFromVecto);

		void SetHalfTone(bool State);

		void LookAt(const Vec3f& From, const Vec3f& At, const Vec3f& Up, float Angle, float Min, float Max);
		void CVM(const std::vector<Vec3f>& Vertex, std::vector<Vec3f>& VertexBuffer, std::vector<Vec2f>& ProjectionBuffer, std::vector<bool>& SightBuffer);
		void BFC(const std::vector<Vec3f>& Vertex, const std::vector<Vec3d>& Face, const std::vector<Vec3f>& Normal, const std::vector<Vec3d>& FaceNormal, std::vector<bool>& FrontFaceBuffer);
		void Sort(const std::vector<Vec3f>& VertexBuffer, std::vector<Vec3d>& FaceBuffer, std::vector<int>& FaceColorBuffer, std::vector<bool>& FrontFaceBuffer);

		bool Init(const Vec2d& Size, PROJECTION Projection = PROJECTION_XY);
		void Draw(const std::vector<Polygon*>& PolygonStream, const std::vector<Light*>& LightStream);
		void Render();
};

extern Renderer renderer;
extern Renderer renderer1;
extern Renderer renderer2;

#endif
