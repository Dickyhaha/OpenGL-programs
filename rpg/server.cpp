#include "server.h"

Server* server_ptr;

Server::Server()
{
}

Server::~Server()
{
}

bool Server::cleanup(const char* message, SOCKET sock, int player_index)
{
	cout << message << endl;

	if(sock != INVALID_SOCKET)
		close(sock);

	if(player_index != -1)
	{
		list<Player>::iterator it = world->getPlayers()->begin();
		for(int i = 0; i < player_index; i++, it++);
		world->killPlayer(it);
	}

	return false;
}

void Server::sendClient(int index)
{
	char message[MAX_MSG_LENGTH] = {0};

	SOCKET client_socket = client_handlers[index].client_socket;

	list<Player>::iterator me = world->getPlayers()->begin();
	for(int i = 0; i < index; i++, me++);

	float* position;
	float* rotation;
	float* color;
	char* name;
	int shot;	
	int hp;

	while(1)
	{
		if(!client_handlers[index].update)
			continue;

		client_handlers[index].update = false;

		world->lock();

		int player_count = world->getPlayerCount();

		sprintf(message, "%d:", player_count - 1);
	
		list<Player>::iterator it = world->getPlayers()->begin();
		for(int i = 0; i < player_count; i++, it++)
		{
			if(it == me)
				continue;

			position = world->getPlayerPosition(it);
			rotation = world->getPlayerRotation(it);
			color = world->getPlayerColor(it);
			shot = world->getPlayerShot(it);
			name = world->getPlayerName(it);
			hp = world->getPlayerHP(it);

			sprintf(message, "%s%s:%d:%f:%f:%f:%f:%f:%f:%f:%f:%f:%d:", message, 
					name, hp,
					position[0], position[1], position[2],
					rotation[0], rotation[1], rotation[2],
					color[0], color[1], color[2], shot);
		}

		world->unlock();

		strcat(message, "@");

		if(send(client_socket, message, strlen(message), 0) < 0)
			break;

		memset(message, 0, sizeof(message));
	}

	client_handlers[index].running = false;
	return;
}

void Server::recvClient(int index)
{
	char message[MAX_MSG_LENGTH] = {0};
	char* ptr;

	SOCKET client_socket = client_handlers[index].client_socket;

	list<Player>::iterator me = world->getPlayers()->begin();
	for(int i = 0; i < index; i++, me++);

	float x, y, z;
	while(recv(client_socket, message, sizeof(message), 0) > 0)
	{
		world->lock();

		world->setPlayerHP(me, atoi(strtok_r(message, ":", &ptr)));

		if(atoi(strtok_r(NULL, ":", &ptr)))
			world->setPlayerShot(me);

		x = atof(strtok_r(NULL, ":", &ptr));
		y = atof(strtok_r(NULL, ":", &ptr));
		z = atof(strtok_r(NULL, ":", &ptr));
		world->setPlayerPosition(me, x, y, z);

		x = atof(strtok_r(NULL, ":", &ptr));
		y = atof(strtok_r(NULL, ":", &ptr));
		z = atof(strtok_r(NULL, ":", &ptr));
		world->setPlayerRotation(me, x, y, z);

		world->unlock();

		memset(message, 0, sizeof(message));
	}

	client_handlers[index].running = false;
	return;
}

void Server::update()
{
	timeval start, now;
	start.tv_sec = start.tv_usec = 0;

	while(1)
	{
		gettimeofday(&now, NULL);
		if((now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec -start.tv_usec) / 1000 < 30)
			continue;
		gettimeofday(&start, NULL);

		//world->update();

		for(unsigned int i = 0; i < client_handlers.size(); i++)
			client_handlers[i].update = true;
	}
}

void Server::manage()
{
	timeval start, now;
	start.tv_sec = start.tv_usec = 0;

	vector<Client_handler>::iterator it;

	while(1)
	{
		gettimeofday(&now, NULL);
		if((now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000 < 10)
			continue;

		gettimeofday(&start, NULL);

		for(it = client_handlers.begin(); it != client_handlers.end();)
		{
			if(it->running)
				it++;
			else
			{
				pthread_join(it->client_send, NULL);
				pthread_join(it->client_recv, NULL);
				cleanup("Client disconnected", it->client_socket, it->index);
				client_handlers.erase(it);
				continue;
			}
		}
	}
}

int Server::initClient(SOCKET client_socket)
{
	char message[MAX_MSG_LENGTH];

	timeval start, now;
	gettimeofday(&start, NULL);

	Player player;

	Client_handler handler;
	handler.running = true;
	handler.update = false;

	player.hp = 100;
	player.shot = false;

	player.position[1] = 2.5f;
	do
	{		
		gettimeofday(&now, NULL);
		if(now.tv_sec - start.tv_sec > 5)
			return cleanup("Random timeout", client_socket);

		player.position[0] = ((float)rand()/RAND_MAX) * 50.0f - 25.0f;	
		player.position[2] = ((float)rand()/RAND_MAX) * 50.0f - 25.0f;

	} while(world->playerCollision(&player));

	player.rotation[0] = 0.0f;
	player.rotation[1] = ((float)rand()/RAND_MAX) * M_PI * 2;
	player.rotation[2] = 0.0f;

	player.velocity[0] = 0.0f;
	player.velocity[1] = 0.0f;
	player.velocity[2] = 0.0f;

	player.color[0] = ((float)rand()/RAND_MAX);
	player.color[1] = ((float)rand()/RAND_MAX);
	player.color[2] = ((float)rand()/RAND_MAX);

	sprintf(message, "%f:%f:%f:%f:%f:%f:%f:%f:%f:",
			player.position[0], player.position[1], player.position[2],
			player.rotation[0], player.rotation[1], player.rotation[2],
			player.color[0],	player.color[1],	player.color[2]);

	while(send(client_socket, message, strlen(message), 0) <= 0)
	{
		if(errno != EAGAIN)
			return cleanup("Send to client failed", client_socket);

		gettimeofday(&now, NULL);
		if(now.tv_sec - start.tv_sec > 5)
			return cleanup("Send info timeout", client_socket);
	}
	
	memset(message, 0, sizeof(message));

	while(recv(client_socket, message, sizeof(message), 0) <= 0)
	{
		if(errno != EAGAIN)
			return cleanup("Recv from client failed", client_socket);

		gettimeofday(&now, NULL);
		if(now.tv_sec - start.tv_sec > 5)
			return cleanup("Recv info timeout", client_socket);
	}

	fcntl(client_socket, F_SETFL, fcntl(client_socket, F_GETFL, 0) & (~O_NONBLOCK));

	world->lock();

	int player_count;
	list<Player>::iterator it = world->getPlayers()->begin();
	for(player_count = 0; player_count < world->getPlayerCount(); player_count++, it++);

	world->setPlayerName(it, message);
	world->setPlayerPosition(it, player.position[0], player.position[1], player.position[2]);
	world->setPlayerRotation(it, player.rotation[0], player.rotation[1], player.rotation[2]);
	world->setPlayerColor(it, player.color[0], player.color[1], player.color[2]);
	world->setPlayerHP(it, player.hp);
	world->setPlayerCount(player_count + 1);

	world->unlock();

	handler.index = player_count;
	handler.client_socket = client_socket;

	client_handlers.push_back(handler);
	pthread_create(&client_handlers[player_count].client_send, NULL, &onSend, (void*)&client_handlers[player_count].index);
	pthread_create(&client_handlers[player_count].client_recv, NULL, &onRecv, (void*)&client_handlers[player_count].index);

	return true;
}

SOCKET Server::initSocket(sockaddr_in* addr)
{
	SOCKET sock;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return cleanup("Construct socket failed", sock);

	int reuse = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)))
		return cleanup("Set reuse address failed", sock);

#ifdef SO_REUSEPORT
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int)))
		return cleanup("Set reuse port failed", sock);
#endif

	fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

	if(bind(sock, (sockaddr*)addr, sizeof(sockaddr)))
		return cleanup("Bind failed", sock);

	if(listen(sock, 10))
		return cleanup("Listen failed", sock);

	return sock;
}

bool Server::init(int port, World* w)
{
	signal(SIGINT, onExit);
	signal(SIGPIPE, onPipe);

	server_ptr = this;
	world = w;
	server_port = port;

	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(server_port);

	return (server_socket = initSocket(&local)) != INVALID_SOCKET;
}

bool Server::SLoop()
{
	SOCKET client_socket;
	sockaddr_in client_address;
	socklen_t length = sizeof(sockaddr);;

	pthread_create(&world_update, NULL, onUpdate, NULL);
	pthread_create(&client_manage, NULL, onManage, NULL);

	while(1)
	{
		if((client_socket = accept(server_socket, (sockaddr*)&client_address, &length)) < 0)
			continue;

		initClient(client_socket);
	}

	return true;
}

void Server::quit()
{
	cleanup("", server_socket);
	exit(0);
}

void Server::pipe()
{
	cout << "Broken Pipe signal caught" << endl;
}

void* onSend(void* ptr)
{
	if(server_ptr != NULL)
		server_ptr->sendClient(*((int*)ptr));
	return NULL;
}

void* onRecv(void* ptr)
{
	if(server_ptr != NULL)
		server_ptr->recvClient(*((int*)ptr));
	return NULL;
}

void* onUpdate(void* ptr)
{
	if(server_ptr != NULL)
		server_ptr->update();
	return NULL;
}

void* onManage(void* ptr)
{
	if(server_ptr != NULL)
		server_ptr->manage();
	return NULL;
}

void onExit(int sig)
{
	if(server_ptr != NULL)
		server_ptr->quit();
}

void onPipe(int sig)
{
	if(server_ptr != NULL)
		server_ptr->pipe();
}
