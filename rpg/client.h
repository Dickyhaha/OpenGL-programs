#ifndef _CLIENT_H_
#define _CLIENT_H_

#ifdef __linux__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#else

#include <winsock2.h>

#endif

#include <pthread.h>
#include <signal.h>

#include "world_define.h"
#include "world.h"

using namespace std;

class Client
{
	private:
		World* world;

		bool running;

		SOCKET server_socket;
		char server_ip[16];
		int server_port;

		char player_name[20];

		int buffer_head;
		int buffer_tail;
		char message_buffer[MAX_BUFFER_LENGTH][MAX_MSG_LENGTH];

		pthread_t send_thread;
		pthread_t recv_thread;
		pthread_t proc_thread;

	public:
		Client();
		~Client();

		bool sendInfo();
		bool recvInfo();
		void procInfo();

		bool cleanup(const char* message);
		bool init(const char* name, const char* ip, int port, World* w);
		void CLoop();
		void quit();
};

void* onSend(void* ptr);
void* onRecv(void* ptr);
void* onProc(void* ptr);
void  onExit(int sig);

#endif
