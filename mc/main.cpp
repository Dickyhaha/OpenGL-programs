#include "gl.h"

#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
	srand(time(NULL));

	World world;
	GL gl;

	cout << "Initializing" << endl;

	if(!world.Init())
	{
		cout << "World init failed" << endl;
		return 0;
	}

	if(!gl.init("Testing", 1000, 1000, 100, 0, 30, &world))
	{
		cout << "GL init failed" << endl;
		return 0;
	}

	gl.GLoop();
	return 0;
}
