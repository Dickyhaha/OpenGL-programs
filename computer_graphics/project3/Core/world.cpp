#include "world.h"

World world;

World::World()
{
}

World::~World()
{
}

void World::PushPolygon()
{
	polygon.push_back(new Polygon);
	PolygonIndex = polygon.size() - 1;
	polygon[PolygonIndex]->SetIndex(PolygonIndex);
}

void World::PopPolygon()
{
	if(polygon.size() == 0)
		return;

	polygon.pop_back();

	if(PolygonIndex >= polygon.size())
		PolygonIndex = polygon.size() - 1;
}

Polygon* World::GetPolygon()
{
	if(polygon.size() == 0)
		return NULL;

	return polygon[PolygonIndex];
}

const std::vector<Polygon*>& World::GetPolygonStream()
{
	return polygon;
}

int World::GetPolygonCount()
{
	return polygon.size();
}

int World::GetPolygonIndex()
{
	return PolygonIndex;
}

void World::FlushPolygon()
{
	polygon.clear();
	PolygonIndex = -1;
}

void World::UpdatePolygon()
{
	for(unsigned int i = 0; i < polygon.size(); i++)
		polygon[i]->Update();
}

void World::SetPolygonIndex(unsigned int Index)
{
	if(Index >= polygon.size() || Index < 0)
		return;

	PolygonIndex = Index;	
}

void World::SavePolygon(const char* filename)
{
	std::ofstream file(filename, ios::out);
	file << polygon.size() << "\n" << std::endl;

	for(unsigned int p = 0; p < polygon.size(); p++)
	{
		file << polygon[p]->GetOBJ() << std::endl;

		file << polygon[p]->GetSpinRate() << std::endl;

		Vec3f point = polygon[p]->GetSpinPoint();
		file << point.x << " " << point.y << " " << point.z << std::endl;

		Vec3f direction = polygon[p]->GetSpinDirection();
		file << direction.x << " " << direction.y << " " << direction.z << std::endl;

		Vec3f diffuse = polygon[p]->GetDiffuseColor();
		file << diffuse.x << " " << diffuse.y << " " << diffuse.z << std::endl;

		Vec3f specular = polygon[p]->GetSpecularColor();
		file << specular.x << " " << specular.y << " " << specular.z << std::endl;
	}

	file.close();
}

void World::LoadPolygon(const char* filename)
{
	Lock();

	polygon.clear();
	std::ifstream file;
	file.open(filename);

	std::ifstream obj;
	char obj_name[100];

	int polygon_count;

	float spin_rate;
	Vec3f spin_point;
	Vec3f spin_plane;

	Vec3f diffuse_color;
	Vec3f specular_color;

	Vec3f translate;

	file >> polygon_count;
	for(int i = 0; i < polygon_count; i++)
	{
		file >> obj_name;

		file >> spin_rate;
		file >> spin_point.x >> spin_point.y >> spin_point.z;
		file >> spin_plane.x >> spin_plane.y >> spin_plane.z;

		file >> diffuse_color.x >> diffuse_color.y >> diffuse_color.z;
		file >> specular_color.x >> specular_color.y >> specular_color.z;

		PushPolygon();

		if(!polygon[i]->LoadModel(obj_name))
		{
			std::cout << "Load model failed" << endl;
			PopPolygon();
			return;
		}

		polygon[i]->SetSpin(spin_point, spin_plane, spin_rate);

		polygon[i]->SetDiffuseColor(diffuse_color);
		polygon[i]->SetSpecularColor(specular_color);
	}

	file.close();

	Unlock();
}

void World::SwapPolygon(unsigned int First, unsigned int Second)
{
	if(First < 0 || First >= polygon.size() || Second < 0 || Second >= polygon.size() || First == Second)
		return;

	Polygon* temp = polygon[First];
	polygon[First] = polygon[Second];
	polygon[Second] = temp;

	polygon[First]->SetIndex(First);
	polygon[Second]->SetIndex(Second);
}

//End of polygon

void World::PushLight()
{
	light.push_back(new Light);
	LightIndex = light.size() - 1;
}

void World::PopLight()
{
	if(light.size() == 0)
		return;

	light.pop_back();

	if(LightIndex >= light.size())
		LightIndex = light.size() - 1;
}

Light* World::GetLight()
{
	if(light.size() == 0)
		return NULL;

	return light[LightIndex];
}

const std::vector<Light*>& World::GetLightStream()
{
	return light;
}

int World::GetLightCount()
{
	return light.size();
}

int World::GetLightIndex()
{
	return LightIndex;
}

void World::FlushLight()
{
	light.clear();
	LightIndex = -1;
}

void World::SetLightIndex(unsigned int Index)
{
	if(Index >= light.size() || Index < 0)
		return;

	LightIndex = Index;	
}

void World::SaveLight(const char* filename)
{
	std::ofstream file(filename, ios::out);
	file << light.size() << "\n" << std::endl;

	LIGHT type;

	int power;

	Vec3f position;
	Vec3f direction;

	Vec3f intensity;

	for(unsigned int l = 0; l < light.size(); l++)
	{
		type = light[l]->GetLightModel();

		if(type == DIFFUSE)
		{
			power = light[l]->GetPower();

			position = light[l]->GetPosition();
			direction = light[l]->GetDirection();
		}

		intensity = light[l]->GetIntensity();

		file << type << endl;

		if(type == DIFFUSE)
		{
			file << power << endl;

			file << position.x << " " << position.y << " " << position.z << endl;
			file << direction.x << " " << direction.y << " " << direction.z << endl;
		}

		file << intensity.x << " " << intensity.y << " " << intensity.z << endl << endl;	
	}

	file.close();
}

void World::LoadLight(const char* filename)
{
	Lock();

	light.clear();
	std::ifstream file;
	file.open(filename);

	std::ifstream obj;

	int light_count;

	LIGHT type;

	int power;

	Vec3f position;
	Vec3f direction;

	Vec3f intensity;

	file >> light_count;
	for(int i = 0; i < light_count; i++)
	{
		file >> type;

		if(type == DIFFUSE)
		{
			file >> power;

			file >> position.x >> position.y >> position.z;
			file >> direction.x >> direction.y >> direction.z;
		}

		file >> intensity.x >> intensity.y >> intensity.z;

		PushLight();

		light[i]->SetLightModel(type);

		if(type == DIFFUSE)
		{
			light[i]->SetPower(power);

			light[i]->SetPosition(position);
			light[i]->SetDirection(direction);
		}

		light[i]->SetIntensity(intensity);
	}

	file.close();

	Unlock();
}

void World::SwapLight(unsigned int First, unsigned int Second)
{
	if(First < 0 || First >= light.size() || Second < 0 || Second >= light.size() || First == Second)
		return;

	Light* temp = light[First];
	light[First] = light[Second];
	light[Second] = temp;
}

//End of light

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
	/*
	   Vec3f rotation = character.GetObjectRotation();

	   if(rotation.x + -ry <= M_PI / 2 && rotation.x + -ry >= -M_PI /2)
	   rotation.x += -ry;

	   rotation.y += rx;

	   if(rotation.y > 2 * M_PI)
	   rotation.y -= 2 * M_PI;
	   if(rotation.y < -2 * M_PI)
	   rotation.y += 2 * M_PI;

	   character.SetObjectRotation(rotation);*/
}

bool World::IsUpdate()
{
	bool temp = updated;
	updated = false;
	return temp;
}

void World::Update()
{
	if(key[27])
		exit(0);

	updated = true;

	if(polygon.size() == 0)
		return;

	UpdatePolygon();
}

void World::Draw()
{
	world.Lock();

	world.Unlock();
}

bool World::Init()
{
	pthread_mutex_init(&world_mutex, NULL);

	memset(key, 0, sizeof(key));
	memset(button, 0, sizeof(button));

	PolygonIndex = -1;
	LightIndex = -1;

	LoadPolygon("polygons.txt");
	LoadLight("lights.txt");

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

