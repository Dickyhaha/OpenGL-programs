#include "polygon.h"

Polygon::Polygon(): Color(WHITE), SpinRate(0.0f), Raster(false), Index(-1)
{
}

Polygon::~Polygon()
{
}

void Polygon::SetVertex(const std::vector<Vec3f>& NewVertex, const std::vector<Vec2d>& NewEdge)
{
	Vertex = NewVertex;
	Edge = NewEdge;

	if(Vertex.size() == 0)
		Raster = false;
	Update();
}

const std::vector<Vec3f>& Polygon::GetVertex()
{
	return Vertex;
}

const std::vector<Vec2d>& Polygon::GetEdge()
{
	return Edge;
}

bool Polygon::GetExtremeProjectionCoor(Vec2f& Min, Vec2f& Max)
{
	if(Vertex.size() == 0)
		return false;

	Min = MinProjectionCoor;
	Max = MaxProjectionCoor;

	return true;
}

bool Polygon::GetExtremeXZProjectionCoor(Vec2f& Min, Vec2f& Max)
{
	if(Vertex.size() == 0)
		return false;

	Min = MinXZProjectionCoor;
	Max = MaxXZProjectionCoor;

	return true;
}

bool Polygon::GetExtremeYZProjectionCoor(Vec2f& Min, Vec2f& Max)
{
	if(Vertex.size() == 0)
		return false;

	Min = MinYZProjectionCoor;
	Max = MaxYZProjectionCoor;

	return true;
}

//Polygon translation
void Polygon::Translate(const Vec3f& Offset)
{
	if(Offset == Vec3f())
		return;

	for(unsigned int i = 0; i < Vertex.size(); i++)
		Vertex[i] = Vertex[i] + Offset;

	Update();
}

//Polygon rotation
void Polygon::Rotate(const Vec3f& Point, const Vec3f& Direction, float Angle)
{
	if(Point == Direction)
		return;

	Vec3f v = Direction - Point;

	float l = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	Vec3f u = Vec3f(v.x / l, v.y /l, v.z / l);
	Vec3f temp;

	float sinA = sin(Angle * 180.0f / M_PI);
	float cosA = cos(Angle * 180.0f / M_PI);

	for(unsigned int i = 0; i < Vertex.size(); i++)
	{
		v = Vertex[i] - Point;

		temp.x = (cosA + u.x * u.x * (1 - cosA)) 		* v.x + 
			(u.x * u.y * (1 - cosA) - u.z * sinA) 	* v.y + 
			(u.x * u.z * (1 - cosA) + u.y * sinA) 	* v.z;

		temp.y = (u.y * u.x * (1 - cosA) + u.z * sinA) 	* v.x +
			(cosA + u.y * u.y * (1 - cosA)) 		* v.y + 
			(u.y * u.z * (1 - cosA) - u.x * sinA) 	* v.z;

		temp.z = (u.z * u.x * (1 - cosA) - u.y * sinA)	* v.x +
			(u.z * u.y * (1 - cosA) + u.x * sinA)	* v.y +
			(cosA + u.z * u.z * (1 - cosA))		* v.z;

		Vertex[i] = temp + Point;
	}

	Update();
}

//Polygon scaling
void Polygon::Scale(const Vec3f& Factor)
{
	if(Factor.x <= 0.0f || Factor.y <= 0.0f || Factor.z <= 0.0f)
		return;

	for(unsigned int i = 0; i < Vertex.size(); i++)
		Vertex[i] = (Vertex[i] - Center) * Factor + Center;

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

//Unchanged, still in the 2d setting
void Polygon::SutherlandHodgeman(const Vec2d& Min, const Vec2d& Max)
{/*
	std::vector<Vec2d> Out = Vertex, In;

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
	Out.push_back(
	Out.push_back(v2);
	}
	else if(code1 && code2)
	Out.push_back(v2);
	else if(code1 && !code2)
	Out.push_back(
	}
	}

	Vertex = Out;
	Update();*/
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

const Vec3d& Polygon::GetColor()
{
	return Color;
}

void Polygon::SetColor(const Vec3d& NewColor)
{
	Color = NewColor;
}

const Vec3f& Polygon::GetCenter()
{
	return Center;
}

void Polygon::SetSpin(const Vec3f& Point, const Vec3f& Direction, float Rate)
{
	SpinPoint = Point;
	SpinDirection = Direction;
	SpinRate = Rate;
}

const Vec3f& Polygon::GetSpinPoint()
{
	return SpinPoint;
}

const Vec3f& Polygon::GetSpinDirection()
{
	return SpinDirection;
}

float Polygon::GetSpinRate()
{
	return SpinRate;
}

void Polygon::SetIndex(int Num)
{
	Index = Num;
	Update();
}

//Update center of polygon and all projection coordinates
void Polygon::Update()
{
	if(Vertex.size() == 0)
		return;

	Vec3f Mean = Vertex[0];
	ProjectionVertex = std::vector<Vec2f>(Vertex.size());
	XZProjectionVertex = ProjectionVertex;
	YZProjectionVertex = ProjectionVertex;

	ProjectionVertex[0] = Vec2f(Vertex[0].x, Vertex[0].y);
	XZProjectionVertex[0] = Vec2f(Vertex[0].x, Vertex[0].z);
	YZProjectionVertex[0] = Vec2f(Vertex[0].z, Vertex[0].y);

	MinProjectionCoor = MaxProjectionCoor = ProjectionVertex[0];
	MinXZProjectionCoor = MaxXZProjectionCoor = XZProjectionVertex[0];
	MinYZProjectionCoor = MaxYZProjectionCoor = YZProjectionVertex[0];

	for(unsigned int i = 1; i < Vertex.size(); i++)
	{
		//Calculate Center point
		Mean = Mean +  Vertex[i];

		//Projection onto xy plane
		ProjectionVertex[i].x = Vertex[i].x;
		ProjectionVertex[i].y = Vertex[i].y;

		//Projection onto xz plane
		//since buffer is in XY coordinate, do a simple transformation
		XZProjectionVertex[i].x = Vertex[i].x;
		XZProjectionVertex[i].y = Vertex[i].z;

		//Projection onto YZ plane
		YZProjectionVertex[i].x = Vertex[i].z;
		YZProjectionVertex[i].y = Vertex[i].y;

		//Calculate extreme world coordinates for NDC usage
		if(ProjectionVertex[i].x < MinProjectionCoor.x)
			MinProjectionCoor.x = ProjectionVertex[i].x;
		if(ProjectionVertex[i].y < MinProjectionCoor.y)
			MinProjectionCoor.y = ProjectionVertex[i].y;

		if(ProjectionVertex[i].x > MaxProjectionCoor.x)
			MaxProjectionCoor.x = ProjectionVertex[i].x;
		if(ProjectionVertex[i].y > MaxProjectionCoor.y)
			MaxProjectionCoor.y = ProjectionVertex[i].y;		

		//Calculate extreme world coordinates for NDC usage XZ
		if(XZProjectionVertex[i].x < MinXZProjectionCoor.x)
			MinXZProjectionCoor.x = XZProjectionVertex[i].x;
		if(XZProjectionVertex[i].y < MinXZProjectionCoor.y)
			MinXZProjectionCoor.y = XZProjectionVertex[i].y;

		if(XZProjectionVertex[i].x > MaxXZProjectionCoor.x)
			MaxXZProjectionCoor.x = XZProjectionVertex[i].x;
		if(XZProjectionVertex[i].y > MaxXZProjectionCoor.y)
			MaxXZProjectionCoor.y = XZProjectionVertex[i].y;

		//Calculate extreme world coordinates for NDC usage YZ
		if(YZProjectionVertex[i].x < MinYZProjectionCoor.x)
			MinYZProjectionCoor.x = YZProjectionVertex[i].x;
		if(YZProjectionVertex[i].y < MinYZProjectionCoor.y)
			MinYZProjectionCoor.y = YZProjectionVertex[i].y;

		if(YZProjectionVertex[i].x > MaxYZProjectionCoor.x)
			MaxYZProjectionCoor.x = YZProjectionVertex[i].x;
		if(YZProjectionVertex[i].y > MaxYZProjectionCoor.y)
			MaxYZProjectionCoor.y = YZProjectionVertex[i].y;
	}

	Center = Vec3f(Mean.x / Vertex.size(), Mean.y / Vertex.size(), Mean.z / Vertex.size());	
}

//Draw projected coordinates to 3 canvas
void Polygon::Draw()
{
	canvas.DrawVertex(ProjectionVertex, Edge, Color);
	canvas1.DrawVertex(XZProjectionVertex, Edge, Color);
	canvas2.DrawVertex(YZProjectionVertex, Edge, Color);

	if(SpinRate != 0.0f)
		Rotate(SpinPoint, SpinDirection, SpinRate);
}
