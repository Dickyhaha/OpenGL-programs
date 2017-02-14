#include "slider.h"


Slider::Slider(int Min, int Max, int Default, bool B): MinValue(Min), MaxValue(Max), DefaultValue(Default), CurrentValue(Default), Border(B)
{
}

Slider::~Slider()
{
}

int Slider::GetMaxValue()
{
	return MaxValue;
}

void Slider::SetMaxValue(int NewValue)
{
	MaxValue = NewValue;
}

int Slider::GetMinValue()
{
	return MinValue;
}

void Slider::SetMinValue(int NewValue)
{
	MinValue = NewValue;
}

int Slider::GetCurrentValue()
{
	return CurrentValue;
}

void Slider::SetCurrentValue(int NewValue)
{
	if(NewValue >= MinValue && NewValue <= MaxValue)
		CurrentValue = NewValue;
}

int Slider::GetDefaultValue()
{
	return DefaultValue;
}

void Slider::SetDefaultValue(int NewValue)
{
	DefaultValue = NewValue;
}

bool Slider::OnTrigger(const Vec2d& MousePosition, int MouseState)
{
	if(MouseState == MOUSEDOWN || MouseState == MOUSEHOLD)
	{
		SetCurrentValue((MousePosition.x - (Position.x + Size.y / 2)) *  MaxValue / (Size.x - Size.y));
		
		Polygon* polygon = world.GetPolygon();
		if(polygon != NULL)
			polygon->Spinning((float) CurrentValue / MaxValue * 45);

		return true;
	}
	return false;
}

void Slider::Update()
{
	Polygon* polygon = world.GetPolygon();
	if(polygon != NULL)
		SetCurrentValue((int)(polygon->GetSpinning() * MaxValue / 45));

	Refresh();
}

void Slider::Refresh()
{
	if(Position == Vec2d(-1, -1) || Size == Vec2d())
		return;

	if(Border)
	{
		hub.DrawLine(Vec2d(Position.x, Position.y), Vec2d(Position.x, Position.y + Size.y));
		hub.DrawLine(Vec2d(Position.x, Position.y), Vec2d(Position.x + Size.x, Position.y));
		hub.DrawLine(Vec2d(Position.x, Position.y + Size.y), Vec2d(Position.x + Size.x, Position.y + Size.y));
		hub.DrawLine(Vec2d(Position.x + Size.x, Position.y), Vec2d(Position.x + Size.x, Position.y + Size.y));
	}

	int Value = CurrentValue * (Size.x - Size.y) / MaxValue + Size.y / 2;

	if(Value > Size.y)
		hub.DrawLine(Vec2d(Position.x + Size.y / 2, Position.y + Size.y / 2), Vec2d(Position.x + Value - Size.y / 2, Position.y + Size.y / 2), ALGO_DDA);
	hub.DrawCircle(Vec2d(Position.x + Value, Position.y + Size.y / 2), Size.y / 2);
	if(Value < Size.x - Size.y)
		hub.DrawLine(Vec2d(Position.x + Value + Size.y / 2, Position.y + Size.y / 2), Vec2d(Position.x + Size.x - Size.y / 2, Position.y + Size.y / 2), ALGO_DDA);
}
