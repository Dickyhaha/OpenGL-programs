#include "world.h"

World::World()
{
}

World::~World()
{
}

void World::updateMouse(int index, int state)
{
	button[index] = state;
}

void World::updateKey(int index, int state)
{
	key[index] = state;
}

void World::updateView(float rx, float ry)
{
	if(me.rotation[0] + -ry <= M_PI / 2 && me.rotation[0] + -ry >= -M_PI /2)
		me.rotation[0] += -ry;

	me.rotation[1] += rx;

	if(me.rotation[1] > 2 * M_PI)
		me.rotation[1] -= 2 * M_PI;
	if(me.rotation[1] < -2 * M_PI)
		me.rotation[1] += 2 * M_PI;
}

bool World::isUpdate()
{
	bool temp = updated;
	updated = false;
	return temp;
}

void World::update()
{
	updated = true;

	lock();

	if(!server_mode)
	{
		if(key[27])
			exit(0);

		if(key['\b'] && !online_mode)
			init(false);

		//Move		
		me.velocity[0] = 0.0f;
		me.velocity[2] = 0.0f;
		if(key['w'])
		{
			me.velocity[0] += WALKING_SPEED * sin(me.rotation[1]);
			me.velocity[2] += WALKING_SPEED * -cos(me.rotation[1]);
		}
		if(key['s'])
		{
			me.velocity[0] -= WALKING_SPEED * sin(me.rotation[1]);
			me.velocity[2] -= WALKING_SPEED * -cos(me.rotation[1]);
		}
		if(key['d'])
		{
			me.velocity[0] += WALKING_SPEED * cos(me.rotation[1]);
			me.velocity[2] += WALKING_SPEED * sin(me.rotation[1]);
		}
		if(key['a'])
		{
			me.velocity[0] -= WALKING_SPEED * cos(me.rotation[1]);
			me.velocity[2] -= WALKING_SPEED * sin(me.rotation[1]);
		}

		if(key[' '] && me.velocity[1] == 0.0f)
			me.velocity[1] = INIT_SPEED;

		playerMove(&me);

		//Shoot
		if(cool_down > 0)
			cool_down--;
		else if(button[LEFT] && sphere_count < MAX_SPHERE_COUNT)
		{
			if(online_mode)
				setShot();

			pushSphere(&me);

			cool_down = 10;
		}

		list<Word>::iterator it2 = words.begin();
		sprintf(it2->content, "HP: %d", me.hp);/*
		  sprintf((it++)->content, "X: %f Y: %f Z: %f", me.position[0], me.position[1], me.position[2]);
		  sprintf(it->content, "RX: %f RY: %f RZ: %f", me.rotation[0], me.rotation[1], me.rotation[2]);*/
	}

	sphereCollision();

	unlock();
}

bool World::init(bool online, bool server)
{
	online_mode = online;
	server_mode = server;
	sphere_count = 0;
	player_count = 0;

	pthread_mutex_init(&world_mutex, NULL);

	if(!server_mode)
	{
		memset(key, 0, sizeof(key));

		ambient_color[0] = 0.3f;
		ambient_color[1] = 0.3f;
		ambient_color[2] = 0.3f;
		ambient_color[3] = 1.0f;

		diffuse_color[0] = 1.0f;
		diffuse_color[1] = 1.0f;
		diffuse_color[2] = 1.0f;
		diffuse_color[3] = 1.0f;

		diffuse_position[0] = 100.0f;
		diffuse_position[1] = 100.0f;
		diffuse_position[2] = 100.0f;
		diffuse_position[3] = 1.0f;

		Word new_word;
		new_word.position[0] = 0.0f;
		new_word.position[1] = 0.0f;
		new_word.color[0] = 1.0f;
		new_word.color[1] = 1.0f;
		new_word.color[2] = 1.0f;
		sprintf(new_word.content, "HP: %d\n", me.hp);	
		words.push_back(new_word);

		new_word.position[0] = 0.0f;
		new_word.position[1] = 30.0f;
		new_word.color[0] = 1.0f;
		new_word.color[1] = 1.0f;
		new_word.color[2] = 1.0f;
		sprintf(new_word.content, "X: %f Y: %f Z: %f\n", me.position[0], me.position[1], me.position[2]);
		words.push_back(new_word);	

		new_word.position[0] = 0.0f;
		new_word.position[1] = 60.0f;
		new_word.color[0] = 1.0f;
		new_word.color[1] = 1.0f;
		new_word.color[2] = 1.0f;
		sprintf(new_word.content, "RX: %f RY: %f RZ: %f\n", me.rotation[0], me.rotation[1], me.rotation[2]);
		words.push_back(new_word);
	}

	spheres = list<Sphere>(MAX_SPHERE_COUNT);
	players = list<Player>(MAX_PLAYER_COUNT);

	for(int i=-3;i<=3;i++)
		for(int j=-3;j<=3;j++)
		{
			if(i == 0 && j == 0)
				continue;

			Cube new_cube;
			new_cube.size = ((float)rand())/RAND_MAX + 1.0f;
			new_cube.position[0] = (float)(i*10);
			new_cube.position[1] = new_cube.size / 2;
			new_cube.position[2] = (float)(j*10);

			new_cube.color[0] = ((float)rand()/RAND_MAX);
			new_cube.color[1] = ((float)rand()/RAND_MAX);
			new_cube.color[2] = ((float)rand()/RAND_MAX);

			cubes.push_back(new_cube);
		}

	if(online_mode || server_mode)
		return true;

	me.position[0] = 0.0f;
	me.position[1] = 2.5f;
	me.position[2] = 0.0f;

	me.rotation[0] = 0.0f;
	me.rotation[1] = 0.0f;
	me.rotation[2] = 0.0f;

	strcpy(me.name, "ME");
	me.shot = false;
	me.hp = 100;

	list<Player>::iterator it = players.begin();
	for(player_count = 0; player_count < 10; player_count++, it++)
	{
		it->position[0] = ((float)rand())/RAND_MAX * 50.0f;
		it->position[1] = 2.5f;
		it->position[2] = ((float)rand())/RAND_MAX * 50.0f;

		it->color[0] = ((float)rand())/RAND_MAX;
		it->color[1] = ((float)rand())/RAND_MAX;
		it->color[2] = ((float)rand())/RAND_MAX;

		it->hp = 100;
	}

	return true;
}

void World::lock()
{
	pthread_mutex_lock(&world_mutex);
}

void World::unlock()
{
	pthread_mutex_unlock(&world_mutex);
}

void World::pushSphere(const Player* player)
{
	list<Sphere>::iterator it = spheres.end();
	it--;

	spheres.splice(spheres.begin(), spheres, it);

	it->position[0] = player->position[0] + sin(player->rotation[1]);
	it->position[1] = player->position[1] + sin(player->rotation[0]) - 0.5f;
	it->position[2] = player->position[2] + -cos(player->rotation[1]);

	it->velocity[0] = SHOOT_POWER * sin(player->rotation[1]);
	it->velocity[1] = SHOOT_POWER * sin(player->rotation[0]);
	it->velocity[2] = SHOOT_POWER * -cos(player->rotation[1]);

	it->color[0] = ((float)rand())/RAND_MAX;
	it->color[1] = ((float)rand())/RAND_MAX;
	it->color[2] = ((float)rand())/RAND_MAX;

	it->radius = RADIUS;

	sphere_count++;
}

void World::killPlayer(list<Player>::iterator it)
{
	players.splice(players.end(), players, it);
	player_count--;
}

void World::sphereCollision()
{
	list<Sphere>::iterator it = spheres.begin();
	for(int i = 0; i < sphere_count; i++)
	{
		if(it->position[1] < 0.0f || it->position[0] * it->position[0] + it->position[2] * it->position[2] > 10000)
		{
			list<Sphere>::iterator temp = it++;
			spheres.splice(spheres.end(), spheres, temp);
			sphere_count--;
			continue;
		}

		int j;
		list<Cube>::iterator it2 = cubes.begin();
		for(j = 0; j < (int)cubes.size(); j++, it2++)
			if(sphereCubeCollision(&(*it), &(*it2)))
				break;
		if(j != (int)cubes.size())
		{
			list<Sphere>::iterator temp = it++;
			spheres.splice(spheres.end(), spheres, temp);
			sphere_count--;
			continue;
		}

		if(spherePlayerCollision(&(*it), &me))
		{
			list<Sphere>::iterator temp = it++;
			spheres.splice(spheres.end(), spheres, temp);
			sphere_count--;
			me.hp -= 10;
			continue;
		}

		list<Player>::iterator it3 = players.begin();
		for(j = 0; j < player_count; j++, it3++)
			if(spherePlayerCollision(&(*it), &(*it3)))
				break;
		if(j != player_count)
		{
			list<Sphere>::iterator temp = it++;
			spheres.splice(spheres.end(), spheres, temp);
			sphere_count--;

			if(online_mode)
				continue;

			it3->hp -= 10;
			if(it3->hp <= 0)
			{
				players.splice(players.end(), players, it3);
				player_count--;
			}

			continue;
		}

		it->position[0] += it->velocity[0];
		it->position[1] += it->velocity[1];
		it->position[2] += it->velocity[2];
		//it->velocity[1] -= GRAVITY;
		it++;
	}

	return;
}

bool World::sphereCubeCollision(const Sphere* sphere, const Cube* cube)
{
	float diffx = abs(sphere->position[0] - cube->position[0]);
	float diffy = abs(sphere->position[1] - cube->position[1]);
	float diffz = abs(sphere->position[2] - cube->position[2]);

	float dist = sphere->radius + cube->size / 2;

	if(diffx >= dist || diffy >= dist || diffz >= dist)
		return false;

	if(diffx < cube->size / 2 || diffy < cube->size / 2|| diffz < cube->size / 2)
		return true;

	return (diffx - cube->size / 2) * (diffx - cube->size / 2) + (diffy - cube->size / 2) * (diffy - cube->size / 2) + (diffz - cube->size / 2) * (diffz - cube->size / 2) < sphere->radius * sphere->radius;
}

bool World::sphereSphereCollision(const Sphere* sphere1, const Sphere* sphere2)
{
	return (sphere1->position[0] - sphere2->position[0]) * (sphere1->position[0] - sphere2->position[0]) +
		(sphere1->position[1] - sphere2->position[1]) * (sphere1->position[1] - sphere2->position[1]) + 
		(sphere1->position[2] - sphere2->position[2]) * (sphere1->position[2] - sphere2->position[2]) > (sphere1->radius + sphere2->radius) * (sphere1->radius + sphere2->radius);
}

bool World::spherePlayerCollision(const Sphere* sphere, const Player* player)
{
	float diffx = abs(sphere->position[0] - player->position[0]);
	float diffy = abs(sphere->position[1] - player->position[1] + 1.25f);
	float diffz = abs(sphere->position[2] - player->position[2]);

	float distx = sphere->radius + 0.5f;
	float disty = sphere->radius + 1.25f;
	float distz = sphere->radius + 0.5f;

	if(diffx >= distx || diffy >= disty || diffz >= distz)
		return false;

	if(diffx < 1.0f || diffy < 2.5f || diffz < 1.0f)
		return true;

	return (diffx - 1.0f) * (diffx - 1.0f) + (diffy - 2.5f) * (diffy - 2.5f) + (diffz - 1.0f) * (diffz - 1.0f) < sphere->radius * sphere->radius;
}

void World::playerMove(Player* player)
{
	float position[3] = {player->position[0], player->position[1], player->position[2]};

	player->position[0] += player->velocity[0];
	player->position[2] += player->velocity[2];

	if(playerCollision(player))
	{
		player->position[0] = position[0];
		player->position[2] = position[2];
	}

	player->position[1] += player->velocity[1];
	player->velocity[1] -= GRAVITY;
	if(player->position[1] < 2.5f)
	{
		player->position[1] = 2.5f;
		player->velocity[1] = 0.0f;
	}
	else if(playerCollision(player))
	{
		player->position[1] = position[1];
		player->velocity[1] = 0.0f;
	}
}

bool World::playerCollision(Player* player)
{
	int i;
	list<Cube>::iterator it = cubes.begin();
	for(i = 0; i < (int)cubes.size(); i++, it++)
		if(playerCubeCollision(player, &(*it)))
			return true;

	list<Player>::iterator it2 = players.begin();
	for(i = 0; i < player_count; i++, it2++)
		if(playerPlayerCollision(player, &(*it2)))
			return true;

	return false;
}

bool World::playerPlayerCollision(const Player* player1, const Player* player2)
{
	return (abs(player1->position[0] - player2->position[0]) < 1.0f &&
			abs(player1->position[1] - player2->position[1]) < 2.5f &&
			abs(player1->position[2] - player2->position[2]) < 1.0f);
}

bool World::playerCubeCollision(const Player* player, const Cube* cube)
{
	return (abs(player->position[0] - cube->position[0]) < 1.0f + cube->size / 2 &&
			abs(player->position[1] - cube->position[1]) < 2.5f + cube->size / 2 &&
			abs(player->position[2] - cube->position[2]) < 1.0f + cube->size / 2);
}

float* World::getAmbientColor()
{
	return ambient_color;
}

float* World::getDiffuseColor()
{
	return diffuse_color;
}

float* World::getDiffusePosition()
{
	return diffuse_position;
}

float* World::getPosition()
{
	return me.position;
}

float* World::getRotation()
{
	return me.rotation;
}

float* World::getVelocity()
{
	return me.velocity;
}

char* World::getName()
{
	return me.name;
}

int World::getHP()
{
	return me.hp;
}

bool World::getShot()
{
	bool temp = me.shot;
	me.shot = false;
	return temp;
}

int World::getSphereCount()
{
	return sphere_count;
}

int World::getPlayerCount()
{
	return player_count;
}

float* World::getSpherePosition(list<Sphere>::iterator it)
{
	return it->position;
}

float* World::getSphereColor(list<Sphere>::iterator it)
{
	return it->color;
}

float World::getSphereRadius(list<Sphere>::iterator it)
{
	return it->radius;
}

float* World::getPlayerPosition(list<Player>::iterator it)
{
	return it->position;
}

float* World::getPlayerRotation(list<Player>::iterator it)
{
	return it->rotation;	
}

float* World::getPlayerVelocity(list<Player>::iterator it)
{
	return it->velocity;
}

char* World::getPlayerName(list<Player>::iterator it)
{
	return it->name;
}

float* World::getPlayerColor(list<Player>::iterator it)
{
	return it->color;	
}

int World::getPlayerHP(list<Player>::iterator it)
{
	return it->hp;
}

bool World::getPlayerShot(list<Player>::iterator it)
{
	bool temp = it->shot;
	it->shot = false;
	return temp;
}

list<Word>* World::getUI()
{
	return &words;
}

list<Cube>* World::getCubes()
{
	return &cubes;
}

list<Sphere>* World::getSpheres()
{
	return &spheres;
}

list<Player>* World::getPlayers()
{
	return &players;
}

void World::setPosition(float x, float y, float z)
{
	me.position[0] = x;
	me.position[1] = y;
	me.position[2] = z;
}

void World::setRotation(float rx, float ry, float rz)
{
	me.rotation[0] = rx;
	me.rotation[1] = ry;
	me.rotation[2] = rz;
}

void World::setVelocity(float x, float y, float z)
{
	me.velocity[0] = x;
	me.velocity[1] = y;
	me.velocity[2] = z;
}

void World::setColor(float r, float g, float b)
{
	me.color[0] = r;
	me.color[1] = g;
	me.color[2] = b;
}

void World::setName(const char* name)
{
	strcpy(me.name, name);
}

void World::setHP(int hp)
{
	me.hp = hp;
}

void World::setShot()
{
	me.shot = true;
}

void World::setSphereCount(int count)
{
	sphere_count = count;
}

void World::setPlayerCount(int count)
{
	player_count = count;
}

void World::setSpherePosition(list<Sphere>::iterator it, float x, float y, float z)
{
	it->position[0] = x;
	it->position[1] = y;
	it->position[2] = z;
}

void World::setSphereColor(list<Sphere>::iterator it, float r, float g, float b)
{
	it->color[0] = r;
	it->color[1] = g;
	it->color[2] = b;
}

void World::setSphereRadius(list<Sphere>::iterator it, float r)
{
	it->radius = r;
}

void World::setPlayerPosition(list<Player>::iterator it, float x, float y, float z)
{
	it->position[0] = x;
	it->position[1] = y;
	it->position[2] = z;
}

void World::setPlayerRotation(list<Player>::iterator it, float rx, float ry, float rz)
{
	it->rotation[0] = rx;
	it->rotation[1] = ry;
	it->rotation[2] = rz;
}

void World::setPlayerVelocity(list<Player>::iterator it, float x, float y, float z)
{
	it->velocity[0] = x;
	it->velocity[1] = y;
	it->velocity[2] = z;
}

void World::setPlayerColor(list<Player>::iterator it, float r, float g, float b)
{
	it->color[0] = r;
	it->color[1] = g;
	it->color[2] = b;
}

void World::setPlayerName(list<Player>::iterator it, const char* name)
{
	strcpy(it->name, name);
}

void World::setPlayerHP(list<Player>::iterator it, int hp)
{
	it->hp = hp;
}

void World::setPlayerShot(list<Player>::iterator it)
{
	it->shot = true;
}
