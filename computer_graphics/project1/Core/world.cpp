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
	for(unsigned int i = 0; i < polygon.size(); i++)
	{
		const std::vector<Vec2f>& Vertex = polygon[i]->GetVertex();
		file << Vertex.size() << " " << polygon[i]->GetRasterize() << std::endl;
		for(unsigned int j = 0; j < Vertex.size(); j++)
			file << Vertex[j].x << " " << Vertex[j].y << std::endl;
		file << std::endl;
	}
	file.close();
}

void World::LoadPolygon(const char* filename)
{
	polygon.clear();
	std::ifstream file;
	file.open(filename);
	int NumPolygon, NumVertex;
	bool Raster;
	std::vector<Vec2f> Vertices;
	Vec2f Vertex;
	file >> NumPolygon;
	for(int i = 0; i < NumPolygon; i++)
	{
		PushPolygon();
		file >> NumVertex >> Raster;
		for(int j = 0; j < NumVertex; j++)
		{
			file >> Vertex.x >> Vertex.y;
			Vertices.push_back(Vertex);
		}
		polygon[PolygonIndex]->SetVertex(Vertices);
		polygon[PolygonIndex]->SetRasterize(Raster);
		Vertices.clear();
	}
}

void World::Swap(unsigned int First, unsigned int Second)
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
	updated = true;

	if(key[27])
		exit(0);
}

void World::Draw()
{
	world.Lock();
	for(unsigned int i = 0; i < polygon.size(); i++)
		polygon[i]->Draw();
	world.Unlock();
}

bool World::Init()
{
	pthread_mutex_init(&world_mutex, NULL);

	memset(key, 0, sizeof(key));
	memset(button, 0, sizeof(button));

	PolygonIndex = -1;

	canvas.DrawLine(Vec2d(300, 300), Vec2d(300, 400));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(350, 400));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(400, 400));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(400, 350));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(400, 300));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(400, 250));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(400, 200));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(350, 200));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(300, 200));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(250, 200));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(200, 200));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(200, 250));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(200, 300));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(200, 350));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(200, 400));
	canvas.DrawLine(Vec2d(300, 300), Vec2d(250, 400));

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

