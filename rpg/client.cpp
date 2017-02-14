#include "client.h"

Client* client_ptr;

Client::Client()
{
}

Client::~Client()
{
}

bool Client::cleanup(const char* message)
{
	cout << message << endl;

	if(server_socket != INVALID_SOCKET)
	{
#ifdef __linux__
		close(server_socket);
#else
		closesocket(server_socket);
#endif
		server_socket = INVALID_SOCKET;
	}

#ifndef __linux__
	WSACleanup();
#endif

	return false;
}

bool Client::sendInfo()
{
	char message[MAX_MSG_LENGTH] = {0};

	float* position;
	float* rotation;
	int shot;
	int hp;

	while(1)
	{
		if(!world->isUpdate())
			continue;

		world->lock();

		if((hp = world->getHP()) == 0)
			continue;

		position = world->getPosition();
		rotation = world->getRotation();
		shot = world->getShot();

		world->unlock();	

		sprintf(message, "%d:%d:%f:%f:%f:%f:%f:%f:",
				hp, shot,
				position[0], position[1], position[2],
				rotation[0], rotation[1], rotation[2]);

		if(send(server_socket, message, strlen(message), 0) < 0)
			break;

		memset(message, 0, sizeof(message));
	}

	running = false;
	return cleanup("");
}

bool Client::recvInfo()
{
	char message[MAX_MSG_LENGTH] = {0};
	int i;
	int j;

	while(recv(server_socket, message, sizeof(message), 0) > 0)
	{

		for(i = 0, j = 0; (message_buffer[buffer_tail][i] = message[j]); i++, j++)
			if(message[j] == '@')
			{
				buffer_tail = (buffer_tail + 1) % MAX_BUFFER_LENGTH;
				i = -1;
			}

		memset(message, 0, sizeof(message));
	}

	running = false;
	return cleanup("");
}

void Client::procInfo()
{
	char* ptr;

	float x, y, z;

	while(running)
	{
		if(buffer_head == buffer_tail || message_buffer[buffer_head][strlen(message_buffer[buffer_head]) - 1] != '@')
			continue;

		world->lock();

		int player_count = atoi(strtok_r(message_buffer[buffer_head], ":", &ptr));
		world->setPlayerCount(player_count);

		list<Player>::iterator it = world->getPlayers()->begin();
		for(int i = 0; i < player_count; i++, it++)
		{
			world->setPlayerName(it, strtok_r(NULL, ":", &ptr));

			world->setPlayerHP(it, atoi(strtok_r(NULL, ":", &ptr)));

			x = atof(strtok_r(NULL, ":", &ptr));
			y = atof(strtok_r(NULL, ":", &ptr));
			z = atof(strtok_r(NULL, ":", &ptr));
			world->setPlayerPosition(it, x, y, z);

			x = atof(strtok_r(NULL, ":", &ptr));
			y = atof(strtok_r(NULL, ":", &ptr));
			z = atof(strtok_r(NULL, ":", &ptr));
			world->setPlayerRotation(it, x, y, z);

			x = atof(strtok_r(NULL, ":", &ptr));
			y = atof(strtok_r(NULL, ":", &ptr));
			z = atof(strtok_r(NULL, ":", &ptr));
			world->setPlayerColor(it, x, y, z);

			if(atoi(strtok_r(NULL, ":", &ptr)))
				world->pushSphere(&(*it));
		}	

		world->unlock();

		memset(message_buffer[buffer_head], 0, sizeof(message_buffer[buffer_head]));

		buffer_head = (buffer_head + 1) % MAX_BUFFER_LENGTH;
	}	
}

bool Client::init(const char* name, const char* ip, int port, World* w)
{
	client_ptr = this;
	world = w;

	signal(SIGINT, onExit);

	strcpy(server_ip, ip);
	server_port = port;

	buffer_head = buffer_tail = 0;
	memset(message_buffer, 0, sizeof(message_buffer));

#ifndef __linux__
	WSAData data;
	if(WSAStartup(0x202, &data))
	{
		cout << "WSAStartup failed" << endl;
		return false;
	}
#endif

	if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return cleanup("Construct socket failed");

#ifdef __linux__
	int reuse = 1;
#else
	char reuse = 1;
#endif

	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)))
		return cleanup("Set reuse address failed");

#ifdef SO_REUSEPORT
	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int)))
		return cleanup("Set reuse port failed");
#endif

	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(server_ip);
	server_address.sin_port = htons(port);

	while(connect(server_socket, (sockaddr*)&server_address, sizeof(sockaddr)) < 0);
	//return cleanup("Connect failed");

	cout << "Conneted to server, waiting for response" << endl;

	char message[MAX_MSG_LENGTH] = {0};
	if(recv(server_socket, message, sizeof(message), 0) <= 0)
		return cleanup("Recv info failed");

	float x, y, z;
	x = atof(strtok(message, ":"));
	y = atof(strtok(NULL, ":"));
	z = atof(strtok(NULL, ":"));
	world->setPosition(x, y, z);

	x = atof(strtok(NULL, ":"));
	y = atof(strtok(NULL, ":"));
	z = atof(strtok(NULL, ":"));
	world->setRotation(x, y, z);

	x = atof(strtok(NULL, ":"));
	y = atof(strtok(NULL, ":"));
	z = atof(strtok(NULL, ":"));
	world->setColor(x, y, z);

	world->setVelocity(0.0f, 0.0f, 0.0f);

	world->setName(name);

	world->setHP(100);

	if(send(server_socket, name, strlen(name), 0) <= 0)
		return cleanup("Send player name failed");

	cout << "\nConnection successful, game starting..." << endl;

	return true;
}

void Client::CLoop()
{
	running = true;

	pthread_create(&send_thread, NULL, onSend, NULL);
	pthread_create(&recv_thread, NULL, onRecv, NULL);
	pthread_create(&proc_thread, NULL, onProc, NULL);
}

void Client::quit()
{
	cleanup("");
	exit(0);
}

void* onSend(void* ptr)
{
	if(client_ptr != NULL)
		client_ptr->sendInfo();
	return NULL;
}

void* onRecv(void* ptr)
{
	if(client_ptr != NULL)
		client_ptr->recvInfo();
	return NULL;
}

void* onProc(void* ptr)
{
	if(client_ptr != NULL)
		client_ptr->procInfo();
	return NULL;
}

void onExit(int sig)
{
	if(client_ptr != NULL)
		client_ptr->quit();
}
