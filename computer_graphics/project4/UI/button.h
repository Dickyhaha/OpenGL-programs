#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "../Core/world_define.h"
#include "textbox.h"
#include "../Core/pixel.h"

class Button: public Textbox
{
	public:
		Button(const char* Default = "", bool B = true);
		~Button();

		virtual void OnTrigger(const Vec2d& MousePosition, int MouseState);
}
