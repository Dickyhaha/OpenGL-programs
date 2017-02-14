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

void World::PushCurve()
{
	curve.push_back(new Curve);
	CurveIndex = curve.size() - 1;
}

void World::PopCurve()
{
	if(curve.size() == 0)
		return;

	curve.pop_back();

	if(CurveIndex >= curve.size())
		CurveIndex = curve.size() - 1;
}

Curve* World::GetCurve()
{
	if(curve.size() == 0)
		return NULL;

	return curve[CurveIndex];
}

const std::vector<Curve*>& World::GetCurveStream()
{
	return curve;
}

int World::GetCurveCount()
{
	return curve.size();
}

int World::GetCurveIndex()
{
	return CurveIndex;
}

void World::FlushCurve()
{
	curve.clear();
	CurveIndex = -1;
}

void World::UpdateCurve()
{
	for(unsigned int c = 0; c < curve.size(); c++)
		curve[c]->Update();
}

void World::SetCurveIndex(unsigned int Index)
{
	if(Index >= curve.size())
		return;
	CurveIndex = Index;	
}

void World::SaveCurve(const char* filename)
{	
	std::ofstream file(filename, ios::out);
	file << curve.size() << "\n" << std::endl;

	for(unsigned int c = 0; c < curve.size(); c++)
	{
		CURVE type = curve[c]->GetCurveType();
		file << type << endl;
		if(type == BSPLINE)
			file << curve[c]->GetDegree() << endl;
		file << curve[c]->GetSegment() << endl;

		Vec3f Color = curve[c]->GetColor();
		file << Color.x << " " << Color.y << " " << Color.z << endl;

		file << curve[c]->GetSpinRate() << endl;

		const std::vector<Vec2f>& points = curve[c]->GetControlPoint();
		file << points.size() << endl;
		for(unsigned int p = 0; p < points.size(); p++)
			file << points[p].x << " " << points[p].y << endl;

		if(type == BSPLINE)
		{
			const std::vector<float>& knots = curve[c]->GetKnot();
			for(unsigned int k = 0; k < knots.size(); k++)
				file << knots[k] << " ";
			cout << endl;
		}
		file << endl;
	}
}

void World::LoadCurve(const char* filename)
{
	Lock();

	curve.clear();
	std::ifstream file;
	file.open(filename);

	int count;
	int segment;
	int point_count;
	int degree;
	CURVE type;

	Vec3f Color;
	Vec2f point;
	float knot;
	float SpinRate;

	file >> count;

	for(int c = 0; c < count; c++)
	{
		PushCurve();

		std::vector<Vec2f> points;
		std::vector<float> knots;

		file >> type;
		if(type == BSPLINE)
			file >> degree;

		file >> segment;
		file >> Color.x >> Color.y >> Color.z;
		file >> SpinRate;

		file >> point_count;
		for(int p = 0; p < point_count; p++)
		{
			file >> point.x >> point.y;
			points.push_back(point);
		}

		if(type == BSPLINE)
		{
			for(int k = 0; k < point_count + degree; k++)
			{
				file >> knot;
				knots.push_back(knot);
			}
		}

		curve[c]->SetColor(Color);
		curve[c]->SetSpinRate(SpinRate);
		curve[c]->SetCurveType(type);
		curve[c]->SetSegment(segment);
		curve[c]->SetControlPoint(points);
		curve[c]->SetDegree(degree);
		curve[c]->SetKnot(knots);
	}

	file.close();

	Unlock();
}

void World::SwapCurve(unsigned int First, unsigned int Second)
{
	if(First < 0 || First >= curve.size() || Second < 0 || Second >= curve.size() || First == Second)
		return;

	Curve* temp = curve[First];
	curve[First] = curve[Second];
	curve[Second] = temp;
}

//End of curve

bool World::GetManipulate()
{
	return Manipulate;
}

void World::UpdateMousePosition(int x, int y)
{
	MousePosition = Vec2d(x, Size.y - y - 1);
}

void World::UpdateMouse(int index, int state)
{
	if(Manipulate && curve.size() != 0 && button[2] == 1 && index == 2 && state == 0)
	{
		//Calculate NDC
		Vec2f Min, Max;
		for(unsigned int c = 0; c < curve.size(); c++)
		{
			const std::vector<Vec2f>& Control = curve[c]->GetControlPoint();
			if(Control.size() != 0)
			{
				Min = Max = Control[0];
				break;
			}
		}
		for(unsigned int c = 0; c < curve.size(); c++)
		{
			const std::vector<Vec2f>& Control = curve[c]->GetControlPoint();
			for(unsigned int p = 0; p < Control.size(); p++)
			{
				if(Control[p].x < Min.x)
					Min.x = Control[p].x;
				else if(Control[p].x > Max.x)
					Max.x = Control[p].x;
				if(Control[p].y < Min.y)
					Min.y = Control[p].y;
				else if(Control[p].y > Max.y)
					Max.y = Control[p].y;
			}
		}

		Vec2f Position = Vec2f(MousePosition);
		Position = Position / (Size - 1) * (Max - Min) + Min;

		if(Select == -1)
		{
			const std::vector<Vec2f>& Control = curve[CurveIndex]->GetControlPoint();
			unsigned int c;
			unsigned int size = Control.size();
			for(c = 0; c < size; c++)
				if(((Control[c] - Min) / (Max - Min) * (Size - 1) - MousePosition).distance() <= 20)
					break;
			if(c == size)
				curve[CurveIndex]->PushPoint(Position);
			else
				curve[CurveIndex]->RemovePoint(c);
		}
	}

	button[index] = state;
}

void World::UpdateKey(int index, int state)
{
	key[index] = state;
	if(key[' '])
		Manipulate = !Manipulate;
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

	if(Manipulate && curve.size() != 0 && button[0])
	{
		//Calculate NDC
		Vec2f Min, Max;
		for(unsigned int i = 0; i < curve.size(); i++)
		{
			const std::vector<Vec2f>& Control = curve[i]->GetControlPoint();
			if(Control.size() == 0)
				continue;
			Min = Max = Control[0];
			break;
		}
		for(unsigned int c = 0; c < curve.size(); c++)
		{
			const std::vector<Vec2f>& Control = curve[c]->GetControlPoint();
			for(unsigned int p = 0; p < Control.size(); p++)
			{
				if(Control[p].x < Min.x)
					Min.x = Control[p].x;
				else if(Control[p].x > Max.x)
					Max.x = Control[p].x;
				if(Control[p].y < Min.y)
					Min.y = Control[p].y;
				else if(Control[p].y > Max.y)
					Max.y = Control[p].y;
			}
		}

		Vec2f Position = Vec2f(MousePosition);
		Position = Position / (Size - 1) * (Max - Min) + Min;

		if(Select == -1)
		{
			const std::vector<Vec2f>& Control = curve[CurveIndex]->GetControlPoint();
			unsigned int c;
			for(c = 0; c < Control.size(); c++)
				if(((Control[c] - Min) / (Max - Min) * (Size - 1) - MousePosition).distance() <= 20)
					break;
			if(c != Control.size() )//&& Control[c].x != Max.x && Control[c].y != Max.y && Control[c].x != Min.x && Control[c].y != Min.y)
				Select = c;
		}
		if(Select != -1)
			curve[CurveIndex]->SetPoint(Select, Position);
	}
	else
		Select = -1;

	UpdatePolygon();
	UpdateCurve();
}

void World::Draw()
{
	world.Lock();

	world.Unlock();
}

bool World::Init(const Vec2d& WindowSize)
{
	pthread_mutex_init(&world_mutex, NULL);

	memset(key, 0, sizeof(key));
	memset(button, 0, sizeof(button));

	Size = WindowSize;

	PolygonIndex = -1;
	LightIndex = -1;
	CurveIndex = -1;

	Select = -1;
	Manipulate = false;

	LoadPolygon("polygons.txt");
	LoadLight("lights.txt");
	LoadCurve("curves.txt");

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

