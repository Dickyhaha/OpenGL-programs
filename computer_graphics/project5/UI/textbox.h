#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "../Core/world_define.h"
#include "interface.h"
#include "../Core/world.h"
#include "../Core/pixel.h"

#include <cstdio>
#include <cstring>

class Textbox: public Interface
{
	private:
		char Text[100];

		bool Border;

	public:
		Textbox(const char* Default = "", bool B = true);
		~Textbox();

		const char* GetText();
		void SetText(const char* NewText);

		int GetNumber();
		void SetNumber(int Value);

		virtual void Update();
		virtual bool OnTrigger(const Vec2d& MousePosition, int MouseState);
		virtual void Refresh();
};

#endif
