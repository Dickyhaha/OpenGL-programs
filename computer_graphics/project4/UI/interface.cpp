#include "interface.h"

Interface::Interface(): TriggerFunction(NULL)
{
}

Interface::~Interface()
{
}

Vec2d Interface::GetInterfacePosition()
{
	return Position;
}

void Interface::SetInterfacePosition(const Vec2d& NewPosition)
{
	Position = NewPosition;
}


Vec2d Interface::GetInterfaceSize()
{
	return Size;
}

void Interface::SetInterfaceSize(const Vec2d& NewSize)
{
	Size = NewSize;
}

Vec3f Interface::GetInterfaceColor()
{
	return Color;
}

void Interface::SetInterfaceColor(const Vec3f& NewColor)
{
	Color = NewColor;
}

void Interface::OnTriggerListener(bool (*Func)(Interface*, const Vec2d&, int))
{
	TriggerFunction = Func;
}

void Interface::OnUpdateListener(void (*Func)(Interface*))
{
	UpdateFunction = Func;
}

bool Interface::OnTrigger(const Vec2d& MousePosition, int MouseState)
{
	if(TriggerFunction != NULL)
		return TriggerFunction(this, MousePosition, MouseState);
	return false;
}

void Interface::Update()
{
	if(UpdateFunction != NULL)
		UpdateFunction(this);
}

