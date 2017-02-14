#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "world_define.h"

class Object
{
	protected:
		Vec3 Position;
		Vec3 Rotation;
		Vec3 Color;

	public:
		Object();
		~Object();

		Vec3 GetObjectPosition();
		void SetObjectPosition(const Vec3& NewPosition);

		Vec3 GetObjectRotation();
		void SetObjectRotation(const Vec3& NewRotation);

		virtual void Update(float DeltaTime);
};

#endif
