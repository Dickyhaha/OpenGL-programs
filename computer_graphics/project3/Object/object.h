#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "../Core/world_define.h"

class Object
{
	protected:
		Vec3f Position;
		Vec3f Rotation;

	public:
		Object();
		~Object();

		Vec3f GetObjectPosition();
		void SetObjectPosition(const Vec3f& NewPosition);

		Vec3f GetObjectRotation();
		void SetObjectRotation(const Vec3f& NewRotation);

		virtual void Update(float DeltaTime);
		virtual void Draw() = 0;
};

#endif
