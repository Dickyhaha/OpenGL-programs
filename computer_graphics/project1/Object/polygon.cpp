#include "polygon.h"

Polygon::Polygon(): Raster(false), Spin(false), Index(-1)
{
}

Polygon::~Polygon()
{
}

void Polygon::SetVertex(const std::vector<Vec2f>& NewVertex)
{
	Vertex = NewVertex;
	if(Vertex.size() == 0)
		Raster = false;
	Update();
}

const std::vector<Vec2f>& Polygon::GetVertex()
{
	return Vertex;
}

//Polygon translation
void Polygon::Translate(const Vec2d& Offset)
{
	pixel.Lock();
	for(unsigned int i = 0; i < Vertex.size(); i++)
	{
		Vertex[i].x += Offset.x;
		Vertex[i].y += Offset.y;
	}
	pixel.Unlock();
	Update();
}

//Polygon rotation
void Polygon::Rotate(float Angle)
{
	float sinA = sin(Angle * M_PI / 180);
	float cosA = cos(Angle * M_PI / 180);

	float dx, dy;

	pixel.Lock();
	for(unsigned int i = 0; i < Vertex.size(); i++)
	{
		dx = Vertex[i].x - Center.x;
		dy = Vertex[i].y - Center.y;	
		Vertex[i].x = cosA * dx - sinA * dy + Center.x;
		Vertex[i].y = sinA * dx + cosA * dy + Center.y;
	}
	pixel.Unlock();

	Update();
}

//Polygon scaling
void Polygon::Scale(float Factorx, float Factory)
{
	if(Factorx <= 0.0f || Factory <= 0.0f)
		return;

	pixel.Lock();
	for(unsigned int i = 0; i < Vertex.size(); i++)
	{
		Vertex[i].x = (Vertex[i].x - Center.x) * Factorx + Center.x;
		Vertex[i].y = (Vertex[i].y - Center.y) * Factory + Center.y;
	}
	pixel.Unlock();

	Update();
}

bool Polygon::IsInside(const Vec2d& Point, const Vec2d& Min, const Vec2d& Max, int CheckCode)
{
	if(CheckCode & UP)
		return Point.y <= Max.y;
	if(CheckCode & DOWN)
		return Point.y >= Min.y;
	if(CheckCode & LEFT)
		return Point.x >= Min.x;
	if(CheckCode & RIGHT)
		return Point.x <= Max.x;

	return true;
}

void Polygon::SutherlandHodgeman(const Vec2d& Min, const Vec2d& Max)
{
	std::vector<Vec2f> Out = Vertex, In;

	for(unsigned int code = LEFT; code <= DOWN; code <<= 1)
	{
		In = Out;
		Out.clear();
		for(unsigned int i = 0; i < In.size(); i++)
		{
			Vec2d v1 = In[i];
			Vec2d v2 = In[(i + 1) % In.size()];

			bool code1 = IsInside(v1, Min, Max, code), code2 = IsInside(v2, Min, Max, code);

			if(!code1 && code2)
			{
				Out.push_back(pixel.Intersect(v1, v2, Min, Max, code));
				Out.push_back(v2);
			}
			else if(code1 && code2)
				Out.push_back(v2);
			else if(code1 && !code2)
				Out.push_back(pixel.Intersect(v1, v2, Min, Max, code));
		}
	}

	Vertex = Out;
	Update();
}

void Polygon::SetRasterize(bool State)
{
	if(Vertex.size() == 0)
		return;

	Raster = State;
	Update();
}

bool Polygon::GetRasterize()
{
	return Raster;
}

void Polygon::SetIndex(int Num)
{
	Index = Num;
	Update();
}

void Polygon::Spinning(float Rate)
{
	Spin = Rate;
}

float Polygon::GetSpinning()
{
	return Spin;
}

void Polygon::Update()
{
	if(Vertex.size() == 0)
		return;

	Vec2f Min = Vertex[0], Max = Vertex[0], Mean = Vertex[0];
	for(unsigned int i = 1; i < Vertex.size(); i++)
	{
		Mean = Mean +  Vertex[i];
		if(Vertex[i].x < Min.x)
			Min.x = Vertex[i].x;
		else if(Vertex[i].x > Max.x)
			Max.x = Vertex[i].x;

		if(Vertex[i].y < Min.y)
			Min.y = Vertex[i].y;
		else if(Vertex[i].y > Max.y)
			Max.y = Vertex[i].y;
	}

	Center = Vec2f(Mean.x / Vertex.size(), Mean.y / Vertex.size());
	pixel.SetPixelPosition(Min);
	pixel.SetPixelSize(Vec2d(Max.x - Min.x + 1, Max.y - Min.y + 1));
	pixel.Clear();
	pixel.DrawPolygon(Vertex);
	if(Raster)
		pixel.Rasterize(Vertex);
}

void Polygon::Draw()
{
	if(Spin != 0.0f)
		Rotate(Spin);
	if(Index >= 0)
	{
		Vec2f Pos = pixel.GetPixelPosition();
		Pos.x += pixel.GetPixelSize().x / 2;
		Pos.y -= 10;
		canvas.DrawNumber(Pos, Index);
	}
	canvas.Draw(pixel);
}
