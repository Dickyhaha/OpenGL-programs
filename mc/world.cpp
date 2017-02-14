#include "world.h"

World::World()
{
}

World::~World()
{
}

void World::UpdateMouse(int index, int state)
{
	button[index] = state;
}

void World::UpdateKey(int index, int state)
{
	key[index] = state;
}

void World::UpdateView(float rx, float ry)
{
	Vec3 rotation = character.GetObjectRotation();

	if(rotation.x + -ry <= M_PI / 2 && rotation.x + -ry >= -M_PI /2)
		rotation.x += -ry;

	rotation.y += rx;

	if(rotation.y > 2 * M_PI)
		rotation.y -= 2 * M_PI;
	if(rotation.y < -2 * M_PI)
		rotation.y += 2 * M_PI;

	character.SetObjectRotation(rotation);
}

bool World::IsUpdate()
{
	bool temp = updated;
	updated = false;
	return temp;
}

void World::Update()
{
	updated = true;

	static timeval start;
	timeval now;

	gettimeofday(&now, NULL);

	if(key[27])
		exit(0);

	Vec3 position = character.GetObjectPosition();
	Vec3 rotation = character.GetObjectRotation();

	if(key['w'])
	{
		position.x += 0.1*sin(rotation.y);
		position.z += 0.1*-cos(rotation.y);
	}
	if(key['s'])
	{
		position.x -= 0.1*sin(rotation.y);
		position.z -= 0.1*-cos(rotation.y);
	}
	if(key['d'])
	{
		position.x += 0.1*cos(rotation.y);
		position.z += 0.1*sin(rotation.y);
	}
	if(key['a'])
	{
		position.x -= 0.1*cos(rotation.y);
		position.z -= 0.1*sin(rotation.y);
	}

	cout << position.x << " " << position.y << " " << position.z << endl;

	if(abs(position.x) <= 50.0f && abs(position.z) <= 50.0f)
		character.SetObjectPosition(position);

	float DeltaTime = (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec) / 100000;

	character.Update(DeltaTime);

	start = now;
}

void World::Draw()
{
	terrain.Draw();
	character.Draw();
}

bool World::Init()
{
	pthread_mutex_init(&world_mutex, NULL);

	memset(key, 0, sizeof(key));
	memset(button, 0, sizeof(button));

	if(!terrain.LoadTerrain("../obj_files/house.obj"))
		cout << "Load model failed" << endl;

	terrain.SetObjectPosition(Vec3(0.0f, 0.0f, 0.0f));
	terrain.SetObjectRotation(Vec3(0.0f, 0.0f, 0.0f));
	terrain.SetMeshColor(Vec3(0.0f, 1.0f, 0.0f));

	character.SetObjectPosition(Vec3(0.0f, 2.5f, 0.0f));
	character.SetObjectRotation(Vec3(0.0f, 0.0f, 0.0f));

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

	return true;
}

void World::Lock()
{
	pthread_mutex_lock(&world_mutex);
}

void World::Unlock()
{
	pthread_mutex_unlock(&world_mutex);
}

float* World::GetAmbientColor()
{
	return ambient_color;
}

float* World::GetDiffuseColor()
{
	return diffuse_color;
}

float* World::GetDiffusePosition()
{
	return diffuse_position;
}

Vec3 World::GetRotation()
{
	return character.GetObjectRotation();
}

Vec3 World::GetPosition()
{
	return character.GetObjectPosition();
}
