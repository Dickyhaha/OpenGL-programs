#ifndef _GL_H_
#define _GL_H_

#include "world_define.h"
#include "world.h"
#include "renderer.h"
#include "../UI/ui.h"
#include "../UI/terminal.h"

using namespace std;

class GL
{
	private:
		int Window;

		int update_ms;
		int window_height;
		int window_width;

	public:
		GL();		
		~GL(); 

		void move(int x, int y);
		void hold(int x, int y);
		void click(int key, int state, int x, int y);
		void keyDown(unsigned char index, int x, int y);
		void keyUp(unsigned char index, int x, int y);
		void resize(int w, int h);
		void update(int value);
		void draw();

		bool init(const char* name, int height, int width, int x, int y, int ms);

		void GLoop();
};

void onMove(int x, int y);
void onHold(int x, int y);
void onClick(int key, int state, int x, int y);
void onKeyDown(unsigned char index, int x, int y);
void onKeyUp(unsigned char index, int x, int y);
void onResize(int w, int h);
void onUpdate(int value);
void onDraw();

#endif
