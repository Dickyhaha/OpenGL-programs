#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include "../Core/world_define.h"
#include "../UI/ui.h"
#include "../Core/gl.h"
#include "../Core/world.h"
#include "../Object/polygon.h"

#include <pthread.h>
#include <cstring>
#include <cstdio>

class Terminal
{
	private:
		pthread_t Thread;

		bool Running;

	public:
		Terminal();
		~Terminal();

		void Run();

		bool Init();

		void Start();
		void Stop();
};

void* OnRun(void* ptr);

extern Terminal terminal;

#endif
