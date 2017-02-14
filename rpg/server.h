#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "world_define.h"
#include "world.h"

using namespace std;

class Server
{
	private:
		World* world;

		SOCKET server_socket;
		int server_port;

		struct Client_handler
		{
			int index;
			bool running;
			bool update;
			SOCKET 	client_socket;

			pthread_t client_send;
			pthread_t client_recv;
		};

		vector<Client_handler> client_handlers;
		
		pthread_t world_update;
		pthread_t client_manage;

	private:
		bool cleanup(const char* message, SOCKET sock, int player_index = -1);
		int  initClient(SOCKET client_socket);
		
		SOCKET initSocket(sockaddr_in* addr);

	public:
		Server();
		~Server();

		void sendClient(int index);
		void recvClient(int index);

		void update();
		void manage();

		bool init(int port, World* w);
		bool SLoop();
		void quit();
		void pipe();
};

void* onSend(void* ptr);
void* onRecv(void* ptr);
void* onUpdate(void* ptr);
void* onManage(void* ptr);
void  onExit(int sig);
void  onPipe(int sig);

#endif
