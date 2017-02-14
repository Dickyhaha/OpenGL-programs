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
	for(unsigned int i = 0; i < polygon.size(); i++)
	{	
		//Rasterization State
		file << polygon[i]->GetRasterize() << std::endl;

		//Spin State
		file << polygon[i]->GetSpinRate() << std::endl;
		const Vec3f& SpinPoint = polygon[i]->GetSpinPoint();
		const Vec3f& SpinDirection = polygon[i]->GetSpinDirection();
		file << SpinPoint.x << " " << SpinPoint.y << " " << SpinPoint.z << std::endl;
		file << SpinDirection.x << " " << SpinDirection.y << " " << SpinDirection.z << std::endl;

		//Color
		const Vec3d& Color = polygon[i]->GetColor();
		file << Color.x << " " << Color.y << " " << Color.z << std::endl;

		//Vertices
		const std::vector<Vec3f>& Vertex = polygon[i]->GetVertex();
		file << Vertex.size() << std::endl;
		for(unsigned int j = 0; j < Vertex.size(); j++)
			file << Vertex[j].x << " " << Vertex[j].y << " " << Vertex[j].z << std::endl;

		//Edges
		const std::vector<Vec2d>& Edge = polygon[i]->GetEdge();
		file << Edge.size() << std::endl;
		for(unsigned int j = 0; j < Edge.size(); j++)
			file << Edge[j].x << " " << Edge[j].y << std::endl;
		file << std::endl;
	}
	file.close();
}

void World::LoadPolygon(const char* filename)
{
	polygon.clear();
	std::ifstream file;
	file.open(filename);

	int NumPolygon;

	bool Raster;

	float SpinRate;
	Vec3f SpinPoint;
	Vec3f SpinDirection;

	Vec3d Color;

	int NumVertex, NumEdge;
	std::vector<Vec3f> Vertices;
	std::vector<Vec2d> Edges;
	Vec3f Vertex;
	Vec2d Edge;

	file >> NumPolygon;
	for(int i = 0; i < NumPolygon; i++)
	{
		PushPolygon();

		file >> Raster;
		polygon[PolygonIndex]->SetRasterize(Raster);

		file >> SpinRate;
		file >> SpinPoint.x >> SpinPoint.y >> SpinPoint.z;
		file >> SpinDirection.x >> SpinDirection.y >> SpinDirection.z;
		polygon[PolygonIndex]->SetSpin(SpinPoint, SpinDirection, SpinRate);

		file >> Color.x >> Color.y >> Color.z;
		polygon[PolygonIndex]->SetColor(Color);

		file >> NumVertex;
		for(int j = 0; j < NumVertex; j++)
		{
			file >> Vertex.x >> Vertex.y >> Vertex.z;
			Vertices.push_back(Vertex);
		}

		file >> NumEdge;
		for(int j = 0; j < NumEdge; j++)
		{
			file >> Edge.x >> Edge.y;
			Edges.push_back(Edge);
		}

		polygon[PolygonIndex]->SetVertex(Vertices, Edges);
		Vertices.clear();
		Edges.clear();
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
	if(key[27])
		exit(0);

	updated = true;

	Vec2f Min, Max;
	Vec2f TempMin, TempMax;

	if(polygon.size() == 0)
		return;

	polygon[0]->GetExtremeProjectionCoor(Min, Max);
	for(unsigned int i = 1; i < polygon.size(); i++)
	{
		if(!polygon[i]->GetExtremeProjectionCoor(TempMin, TempMax))
			continue;

		if(TempMin.x < Min.x)
			Min.x = TempMin.x;
		if(TempMin.y < Min.y)
			Min.y = TempMin.y;

		if(TempMax.x > Max.x)
			Max.x = TempMax.x;
		if(TempMax.y > Max.y)
			Max.y = TempMax.y;
	}

	canvas.SetExtremeWorldCoor(Min, Max);

	polygon[0]->GetExtremeXZProjectionCoor(Min, Max);
	for(unsigned int i = 1; i < polygon.size(); i++)
	{
		if(!polygon[i]->GetExtremeXZProjectionCoor(TempMin, TempMax))
			continue;

		if(TempMin.x < Min.x)
			Min.x = TempMin.x;
		if(TempMin.y < Min.y)
			Min.y = TempMin.y;

		if(TempMax.x > Max.x)
			Max.x = TempMax.x;
		if(TempMax.y > Max.y)
			Max.y = TempMax.y;
	}

	canvas1.SetExtremeWorldCoor(Min, Max);

	polygon[0]->GetExtremeYZProjectionCoor(Min, Max);
	for(unsigned int i = 1; i < polygon.size(); i++)
	{
		if(!polygon[i]->GetExtremeYZProjectionCoor(TempMin, TempMax))
			continue;

		if(TempMin.x < Min.x)
			Min.x = TempMin.x;
		if(TempMin.y < Min.y)
			Min.y = TempMin.y;

		if(TempMax.x > Max.x)
			Max.x = TempMax.x;
		if(TempMax.y > Max.y)
			Max.y = TempMax.y;
	}

	canvas2.SetExtremeWorldCoor(Min, Max);
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

