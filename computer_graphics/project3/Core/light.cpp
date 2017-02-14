#include "light.h"

Light::Light(): LightModel(AMBIENT), Power(2)
{
}

Light::~Light()
{
}

Vec3f Light::GetPosition()
{
	return Position;
}

void Light::SetPosition(const Vec3f& NewPosition)
{
	Position = NewPosition;
}

Vec3f Light::GetDirection()
{
	return Direction;
}

void Light::SetDirection(const Vec3f& NewDirection)
{
	Direction = NewDirection;
}

Vec3f Light::GetIntensity()
{
	return Intensity;
}

void Light::SetIntensity(const Vec3f& NewIntensity)
{
	Intensity = NewIntensity;
}

LIGHT Light::GetLightModel()
{
	return LightModel;
}

void Light::SetLightModel(LIGHT NewLightModel)
{
	LightModel = NewLightModel;
}

int Light::GetPower()
{
	return Power;
}

void Light::SetPower(int NewPower)
{
	Power = NewPower;
}

Vec3f Light::TrueColor(const Vec3f& Point, const Vec3f& Normal, const Vec3f& FromVector, const Vec3f& DiffuseColor, const Vec3f& SpecularColor)
{
	if(LightModel == AMBIENT)
		return Intensity;

	Vec3f LightVector 		= (Position - Point).normalize();
	float cosA 				= (Normal * LightVector).sum();
	Vec3f ReflectionVector 	= Normal * cosA * 2 - LightVector;
	float distance			= (FromVector - Point).distance();
	Vec3f ViewVector 		= (FromVector - Point).normalize();
	float cosB 				= (ReflectionVector * ViewVector).sum();

	if(((LightVector * Normal).sum() < 0.0f) || ((ViewVector * Normal).sum() < 0.0f))
		return Vec3f();

	float K = (Position - Point).distance();

	Vec3f I;
	if((ReflectionVector * ViewVector).sum() <= 0.0f)
		I = Intensity / (distance + K) * (DiffuseColor * cosA);
	else
		I = Intensity / (distance + K) * (DiffuseColor * cosA + SpecularColor * pow(cosB, Power));

	if(I.x < 0.0f || I.y < 0.0f || I.z < 0.0f)
		return Vec3f();

	return I;
}
