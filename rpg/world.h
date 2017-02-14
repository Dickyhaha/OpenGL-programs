#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <pthread.h>

#include "world_define.h"

using namespace std;

class World
{
	private:
		//--Client only--//
		int key[259];
		int button[3];

		bool online_mode;
		bool server_mode;
		bool updated;

		float ambient_color[4];
		float diffuse_color[4];
		float diffuse_position[4];

		Player me;
		int   cool_down;

		//--Client and server--//
		int player_count;
		int sphere_count;

		list<Word> words;
		list<Cube> cubes;

		list<Sphere> spheres;
		list<Player> players;

		pthread_mutex_t world_mutex;

	private:
		bool sphereCubeCollision(const Sphere* sphere, const Cube* cube);
		bool sphereSphereCollision(const Sphere* sphere1, const Sphere* sphere2);
		bool spherePlayerCollision(const Sphere* sphere, const Player* player);

		bool playerPlayerCollision(const Player* player1, const Player* player2);
		bool playerCubeCollision(const Player* player, const Cube* cube);

	public:
		World();
		~World();

		void updateMouse(int index, int state);
		void updateKey(int index, int state);
		void updateView(float rx, float ry);

		bool isUpdate();
		void update();	
		bool init(bool online = false, bool server_mode = false);

		void lock();
		void unlock();

		void pushSphere(const Player* player);
		void killPlayer(list<Player>::iterator it);

		void playerMove(Player* player);
		bool playerCollision(Player* player);
		void sphereCollision();

		float* getAmbientColor();
		float* getDiffuseColor();
		float* getDiffusePosition();

		//Gets
		float*  getPosition();
		float*  getRotation();
		float*  getVelocity();
		char*	getName();
		int 	getHP();
		bool 	getShot();

		//List
		int getSphereCount();
		int getPlayerCount();

		list<Word>* 	getUI();
		list<Cube>* 	getCubes();
		list<Sphere>* 	getSpheres();
		list<Player>* 	getPlayers();

		float* 	getSpherePosition(list<Sphere>::iterator it);
		float* 	getSphereColor(list<Sphere>::iterator it);
		float	getSphereRadius(list<Sphere>::iterator it);

		float* 	getPlayerPosition(list<Player>::iterator it);
		float* 	getPlayerRotation(list<Player>::iterator it);
		float*  getPlayerVelocity(list<Player>::iterator it);
		float* 	getPlayerColor(list<Player>::iterator it);
		char* 	getPlayerName(list<Player>::iterator it);
		int		getPlayerHP(list<Player>::iterator it);	
		bool 	getPlayerShot(list<Player>::iterator it);

		//Sets
		void setPosition(float x, float y, float z);
		void setRotation(float rx, float ry, float rz);
		void setVelocity(float x, float y, float z);
		void setColor(float r, float g, float b);
		void setName(const char* name);
		void setHP(int hp);
		void setShot();

		//List
		void setSphereCount(int count);
		void setPlayerCount(int count);

		void setSpherePosition(list<Sphere>::iterator it, float x, float y, float z);
		void setSphereColor(list<Sphere>::iterator it, float r, float g, float b);
		void setSphereRadius(list<Sphere>::iterator it, float r);

		void setPlayerPosition(list<Player>::iterator it, float x, float y, float z);
		void setPlayerRotation(list<Player>::iterator it, float rx, float ry, float rz);
		void setPlayerVelocity(list<Player>::iterator it, float x, float y, float z);
		void setPlayerColor(list<Player>::iterator it, float r, float g, float b);
		void setPlayerName(list<Player>::iterator it, const char* name);
		void setPlayerHP(list<Player>::iterator it, int hp);
		void setPlayerShot(list<Player>::iterator it);

};

#endif
