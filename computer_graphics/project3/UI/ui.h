#ifndef _UI_H_
#define _UI_H_

#include "../Core/world_define.h"
#include "terminal.h"
#include "interface.h"
#include "../Object/polygon.h"

class Interface;

class UI
{
	protected:
		int Window;
		int UpdateMs;

		Vec2d WindowPosition;
		Vec2d WindowSize;

		std::vector<Interface*> interface;

	public:
		UI();
		~UI();

		bool Interact(const Vec2d& MousePosition, int MouseState);

		void Move(int x, int y);
		void Hold(int x, int y);
		void Click(int Key, int State, int x, int y);
		void KeyDown(unsigned char Index, int x, int y);
		void KeyUp(unsigned char Index, int x, int y);
		void Resize(int x, int y);
		void Update(int Value);
		void Draw();
		
		bool Init(const char* Name, const Vec2d& Position, const Vec2d& Size, int Ms);

		void ULoop();
};

void OnMove(int x, int y);
void OnHold(int x, int y);
void OnClick(int Key, int State, int x, int y);
void OnResize(int width, int height);
void OnUpdate(int Value);
void OnDraw();

extern UI ui;

#endif
