#include "object.h"

Object::Object()
{
}

Object::~Object()
{
}

Vec3f Object::GetObjectPosition()
{
	return Position;
}

void Object::SetObjectPosition(const Vec3f& NewPosition)
{
	Position = NewPosition;
}

Vec3f Object::GetObjectRotation()
{
	return Rotation;
}

void Object::SetObjectRotation(const Vec3f& NewRotation)
{
	Rotation = NewRotation;
}

void Object::Update(float DeltaTime)
{

}
