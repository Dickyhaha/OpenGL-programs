#include "Core/world_define.h"
#include "UI/slider.h"
#include "UI/textbox.h"
#include "Object/polygon.h"
#include "Core/world.h"

/*
void SetNum(Interface* interface)
{
	Textbox* textbox = (Textbox*)interface;

	int num = world.GetPolygonIndex();
	textbox->SetNumber(num == -1?99: num + 1);
}

void SetRed(Interface* interface)
{
	Slider* slider = (Slider*)interface;
	
	Polygon* polygon = 	world.GetPolygon();
	if(polygon == NULL)
		return;

	slider->SetCurrentValue(polygon->GetDiffuseColor().x * 100.0f);
}

void SetGreen(Interface* interface)
{
	Slider* slider = (Slider*)interface;
	
	Polygon* polygon = 	world.GetPolygon();
	if(polygon == NULL)
		return;

	slider->SetCurrentValue(polygon->GetDiffuseColor().y * 100.0f);
}

void SetBlue(Interface* interface)
{
	Slider* slider = (Slider*)interface;
	
	Polygon* polygon = 	world.GetPolygon();
	if(polygon == NULL)
		return;

	slider->SetCurrentValue(polygon->GetDiffuseColor().z * 100.0f);
}

bool Red(Interface* interface, const Vec2d& MousePosition, int MouseState)
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

	Vec3f Color = polygon->GetDiffuseColor();
	polygon->SetDiffuseColor(Vec3f((float)Val / 100.0f, Color.y, Color.z));

	return true;
}

bool Green(Interface* interface, const Vec2d& MousePosition, int MouseState)
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

	Vec3f Color = polygon->GetDiffuseColor();
	polygon->SetDiffuseColor(Vec3f(Color.x, (float)Val / 100.0f, Color.z));

	return true;
}

bool Blue(Interface* interface, const Vec2d& MousePosition, int MouseState)
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

	Vec3f Color = polygon->GetDiffuseColor();
	polygon->SetDiffuseColor(Vec3f(Color.x, Color.y, (float)Val / 100.0f));

	return true;
}

//Light
void LightSetNum(Interface* interface)
{
	Textbox* textbox = (Textbox*)interface;

	int num = world.GetLightIndex();
	textbox->SetNumber(num == -1?99: num + 1);
}

void LightSetRed(Interface* interface)
{
	Slider* slider = (Slider*)interface;
	
	Light* light = 	world.GetLight();
	if(light == NULL)
		return;

	slider->SetCurrentValue(light->GetIntensity().x * 100);
}

void LightSetGreen(Interface* interface)
{
	Slider* slider = (Slider*)interface;
	
	Light* light = 	world.GetLight();
	if(light == NULL)
		return;

	slider->SetCurrentValue(light->GetIntensity().y * 100);
}

void LightSetBlue(Interface* interface)
{
	Slider* slider = (Slider*)interface;
	
	Light* light = 	world.GetLight();
	if(light == NULL)
		return;

	slider->SetCurrentValue(light->GetIntensity().z * 100);
}

bool LightRed(Interface* interface, const Vec2d& MousePosition, int MouseState)
{
	if(MouseState != MOUSEDOWN && MouseState != MOUSEHOLD)
		return false;

	Slider* slider = (Slider*)interface;
	Vec2d NewPosition = MousePosition - slider->GetInterfacePosition();
	int Val = NewPosition.x * slider->GetMaxValue() / slider->GetInterfaceSize().x;
	
	slider->SetCurrentValue(Val);
	
	Light* light = 	world.GetLight();
	if(light == NULL)
		return true;

	Vec3f Color = light->GetIntensity();
	light->SetIntensity(Vec3f((float)Val / 100.0f, Color.y, Color.z));

	return true;
}

bool LightGreen(Interface* interface, const Vec2d& MousePosition, int MouseState)
{
	if(MouseState != MOUSEDOWN && MouseState != MOUSEHOLD)
		return false;

	Slider* slider = (Slider*)interface;
	Vec2d NewPosition = MousePosition - slider->GetInterfacePosition();
	int Val = NewPosition.x * slider->GetMaxValue() / slider->GetInterfaceSize().x;
	
	slider->SetCurrentValue(Val);
	
	Light* light = 	world.GetLight();
	if(light == NULL)
		return true;

	Vec3f Color = light->GetIntensity();
	light->SetIntensity(Vec3f(Color.x, (float)Val / 100.0f, Color.z));

	return true;
}

bool LightBlue(Interface* interface, const Vec2d& MousePosition, int MouseState)
{
	if(MouseState != MOUSEDOWN && MouseState != MOUSEHOLD)
		return false;

	Slider* slider = (Slider*)interface;
	Vec2d NewPosition = MousePosition - slider->GetInterfacePosition();
	int Val = NewPosition.x * slider->GetMaxValue() / slider->GetInterfaceSize().x;
	
	slider->SetCurrentValue(Val);
	
	Light* light = 	world.GetLight();
	if(light == NULL)
		return true;

	Vec3f Color = light->GetIntensity();
	light->SetIntensity(Vec3f(Color.x, Color.y, (float)Val / 100.0f));

	return true;
}*/
