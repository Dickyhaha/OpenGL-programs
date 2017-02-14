#include "server.h"
#include "world.h"

int main(int argc, char** argv)
{
	if(argc < 2)
		return 0;

	World world;
	Server server;

	if(!world.init(false, true))
		return 0;

	if(!server.init(atoi(argv[1]), &world))
		return 0;

	server.SLoop();

	return 0;
}
