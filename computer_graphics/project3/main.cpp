#include "Core/gl.h"
#include "UI/ui.h"

#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
	int width = 1000, height = 1000;

	srand(time(NULL));

	GL gl;

	cout << "ECS 175 Project 1" << endl;

	if(!gl.init("ECS 175 Canvas", width, height, 1000, 0, 30))
	{
		cout << "GL init failed" << endl;
		return 0;
	}	
	
	if(!ui.Init("ECS 175 GUI", Vec2d(0, 0), Vec2d(500, 200), 30))
	{
		cout << "UI init failed" << endl;
		return 0;
	}

	ui.ULoop();
	gl.GLoop();
	return 0;
}
