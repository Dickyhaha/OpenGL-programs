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
		hub.DrawLine(Vec2d(Position.x + Size.y / 2, Position.y + Size.y / 2), Vec2d(Position.x + Value - Size.y / 2, Position.y + Size.y / 2));
	hub.DrawCircle(Vec2d(Position.x + Value, Position.y + Size.y / 2), Size.y / 2);
	if(Value < Size.x - Size.y)
		hub.DrawLine(Vec2d(Position.x + Value + Size.y / 2, Position.y + Size.y / 2), Vec2d(Position.x + Size.x - Size.y / 2, Position.y + Size.y / 2));
}
