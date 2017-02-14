#include "Core/world_define.h"
#include "UI/slider.h"
#include "UI/textbox.h"
#include "Object/polygon.h"
#include "Core/world.h"

bool SliderTrigger(Interface* interface, const Vec2d& MousePosition, int MouseState)
{
	if(MouseState != MOUSEDOWN && MouseState != MOUSEHOLD)
		return false;

	Slider* slider = (Slider*)interface;
	Vec2d NewPosition = MousePosition - slider->GetInterfacePosition();
	int Val = NewPosition.x * slider->GetMaxValue() / slider->GetInterfaceSize().x;
	
	slider->SetCurrentValue(Val);
	
	Polygon* polygon = 	world.GetPolygon();
	if(polygon == NULL)
		return true;

	float factor = (float)(Val) / 50;

	polygon->Scale(Vec3f(factor, factor, factor));
	
	return true;
}
