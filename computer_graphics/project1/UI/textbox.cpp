#include "textbox.h"
#include "slider.h"

Textbox::Textbox(const char* Default, bool B): Border(B)
{
	SetText(Default);
}

Textbox::~Textbox()
{
}

const char* Textbox::GetText()
{
	return Text;
}

void Textbox::SetText(const char* NewText)
{
	strcpy(Text, NewText);
}

int Textbox::GetNumber()
{
	return atoi(Text);
}

void Textbox::SetNumber(int Value)
{
	sprintf(Text, "%d", Value);
}

void Textbox::Update()
{
	int Index = world.GetPolygonIndex();
	if(Index == -1)
		Index = 99;
	SetNumber(Index);
	Refresh();
}

bool Textbox::OnTrigger(const Vec2d& MousePosition, int MouseState)
{
	return false;
}

void Textbox::Refresh()
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

	hub.DrawNumber(Vec2d(Position.x + Size.x / 2, Position.y + Size.y / 2), atoi(Text));
}
