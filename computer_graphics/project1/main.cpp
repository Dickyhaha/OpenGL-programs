#include "Core/gl.h"
#include "UI/ui.h"

#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
	int width = 600, height = 600;

	if(argc == 3)
	{
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	srand(time(NULL));

	GL gl;

	cout << "ECS 175 Project 1" << endl;

	if(!gl.init("ECS 175 Canvas", height, width, 250, 250, 30))
	{
		cout << "GL init failed" << endl;
		return 0;
	}	
	
	if(!ui.Init("ECS 175 GUI", Vec2d(1500, 0), Vec2d(250, 125), 30))
	{
		cout << "UI init failed" << endl;
		return 0;
	}

	ui.ULoop();
	gl.GLoop();
	return 0;
}
