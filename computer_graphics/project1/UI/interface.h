#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "../Core/world_define.h"
#include "../Core/pixel.h"

class Interface
{
	protected:
		Vec2d Position;
		Vec2d Size;

		Vec3f Color;

		bool (*TriggerFunction)(void);

	public:
		Interface();
		virtual ~Interface();

		Vec2d GetInterfacePosition();
		void SetInterfacePosition(const Vec2d& NewPosition);

		Vec2d GetInterfaceSize();
		void SetInterfaceSize(const Vec2d& NewSize);

		Vec3f GetInterfaceColor();
		void SetInterfaceColor(const Vec3f& Newcolor);

		void Connect(bool (*Func)(void));

		bool OnTrigger(const Vec2d& MousePosition, int MouseState);

		virtual void Update();
		virtual void Refresh() = 0;
};

#endif
