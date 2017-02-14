#include "gl.h"
#include "client.h"

#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
	srand(time(NULL));

	bool online_mode = (argc > 1);

	World world;
	Client client;
	GL gl;

	cout << "Initializing" << endl;

	if(!world.init(online_mode))
	{
		cout << "World init failed" << endl;
		return 0;
	}

	if(online_mode && !client.init("Dickyhaha", argv[1], atoi(argv[2]), &world))
	{
		cout << "Client init failed" << endl;
		return 0;
	}

	if(!gl.init("Testing", 1000, 1000, 100, 0, 30, &world))
	{
		cout << "GL init failed" << endl;
		return 0;
	}

	if(online_mode)
		client.CLoop();

	gl.GLoop();
	return 0;
}
