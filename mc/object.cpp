#include "object.h"

Object::Object()
{
}

Object::~Object()
{
}

Vec3 Object::GetObjectPosition()
{
	return Position;
}

void Object::SetObjectPosition(const Vec3& NewPosition)
{
	Position = NewPosition;
}

Vec3 Object::GetObjectRotation()
{
	return Rotation;
}

void Object::SetObjectRotation(const Vec3& NewRotation)
{
	Rotation = NewRotation;
}

void Object::Update(float DeltaTime)
{

}
