#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "world_define.h"
#include "pixel.h"

class Light
{
	private:
		Vec3f Position;
		Vec3f Direction;

		Vec3f Intensity;

		LIGHT LightModel;

		float C;
		int Power;

	public:
		Light();
		~Light();

		Vec3f GetPosition();
		void SetPosition(const Vec3f& NewPosition);

		Vec3f GetDirection();
		void SetDirection(const Vec3f& NewDirection);

		Vec3f GetIntensity();
		void SetIntensity(const Vec3f& NewIntensity);

		LIGHT GetLightModel();
		void SetLightModel(LIGHT NewLightModel);

		int GetPower();
		void SetPower(int NewPower);

		Vec3f TrueColor(const Vec3f& Point, const Vec3f& Normal, const Vec3f& FromVector, const Vec3f& DiffuseColor, const Vec3f& SpecularColor);
		
};

#endif
