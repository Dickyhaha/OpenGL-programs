#include "pixel.h"

Pixel canvas;
Pixel hub;

Pixel canvas1;
Pixel canvas2;

Pixel::Pixel(): Buffer(NULL), Size(Vec2d()), Position(Vec2d()), ClipMin(Vec2d(-1, -1)), ClipMax(Vec2d(-1, -1))
{
	pthread_mutex_init(&pixel_mutex, NULL);
}

Pixel::~Pixel()
{
	if(Buffer != NULL)
	{
		delete[] Buffer;
		Buffer = NULL;
	}
}

void Pixel::Lock()
{
	pthread_mutex_lock(&pixel_mutex);
}

void Pixel::Unlock()
{
	pthread_mutex_unlock(&pixel_mutex);
}

void Pixel::SetErase(bool State)
{
	Erase = State;
}

void Pixel::SetExtremeWorldCoor(const Vec2f& Min, const Vec2f& Max)
{
	MinWorldCoor = Min;
	MaxWorldCoor = Max;
}

PROJECTION Pixel::GetProjectionPlane()
{
	return ProjectionPlane;
}

void Pixel::SetProjectionPlane(PROJECTION Projection)
{
	ProjectionPlane = Projection;
}

Vec2d Pixel::GetPixelPosition() const
{
	return Position;
}

void Pixel::SetPixelPosition(const Vec2d& NewPosition)
{
	Position = NewPosition;
}

Vec2d Pixel::GetPixelSize() const
{
	return Size;
}

void Pixel::SetPixelSize(const Vec2d& NewSize)
{
	if(Size != NewSize)
	{
		Size = NewSize;
		if(Buffer != NULL)
			delete[] Buffer;	
		Buffer = new float[Size.x * Size.y * 3];
	}
}

Vec3d Pixel::GetPixelColor(const Vec2d& PixelPosition) const
{
	if(PixelPosition.x < 0 || PixelPosition.x >= Size.x || PixelPosition.y < 0 || PixelPosition.y >= Size.y)
		return Vec3d();

	int position = (PixelPosition.y * Size.x + PixelPosition.x) * 3;
	return Vec3d(Buffer[position], Buffer[position + 1], Buffer[position + 2]);
}

//Actual modification of the pixel buffer
bool Pixel::SetPixelColor(const Vec2d& PixelPosition, const Vec3d& NewColor)
{	
	if(PixelPosition.x < 0 || PixelPosition.x >= Size.x || PixelPosition.y < 0 || PixelPosition.y >= Size.y)
		return false;

	int position = (PixelPosition.y * Size.x + PixelPosition.x) * 3;
	Buffer[position    ] = (float)NewColor.x / 255;
	Buffer[position + 1] = (float)NewColor.y / 255;
	Buffer[position + 2] = (float)NewColor.z / 255;

	if(Buffer[position] > 1)
		Buffer[position] = 1;
	if(Buffer[position + 1] > 1)
		Buffer[position + 1] = 1;
	if(Buffer[position + 2] > 1)
		Buffer[position + 2] = 1;

	return true;
}

void Pixel::Swap(Vec2d& Start, Vec2d& End, int& dx, int& dy)
{
	if(abs(dy) > abs(dx) && Start.x < End.x)
		return;
	else if(abs(dy) <= abs(dx) && Start.y < End.y)
		return;

	Vec2d Temp = Start;
	Start = End;
	End = Temp;

	dy = End.y - Start.y;
	dx = End.x - Start.x;
}

//If line is horizontal, no algorithm is needed
void Pixel::Horizontal(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color)
{
	int step = Start.x > End.x? -1: 1;
	for(int x = Start.x; (step == 1 && x <= End.x) || (step == -1 && x >= End.x); x += step)
		SetPixelColor(Vec2d(x, Start.y), Color);
}

//If line is vertical, no algorithm is needed
void Pixel::Vertical(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color)
{
	int step = Start.y > End.y? -1: 1;
	for(int y = Start.y; (step == 1 && y <= End.y) || (step == -1 && y >= End.y); y += step)
		SetPixelColor(Vec2d(Start.x, y), Color);
}

//If line is straight, no algorithm is needed
void Pixel::Straight(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color)
{
	int stepx = Start.x > End.x? -1: 1;
	int stepy = Start.y > End.y? -1: 1;
	for(int x = Start.x, y = Start.y; (stepx == 1 && x <= End.x) || (stepx == -1 && x >= End.x); x += stepx, y += stepy)
		SetPixelColor(Vec2d(x, y), Color);
}

//Implement DDA line drawing
void Pixel::DDA(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color)
{	
	float m = fabs(((float)dy)/dx);
	if(m <= 1)
	{
		float y = Start.y;
		int step = Start.x > End.x? -1: 1;
		for(int x = Start.x; (step == 1 && x <= End.x) || (step == -1 && x >= End.x); x += step)
		{
			SetPixelColor(Vec2d(x, (int)y), Color);
			y += m;
		}
	}
	else 
	{
		m = 1/m;
		float x = Start.x;	
		int step = Start.y > End.y? -1: 1;
		for(int y = Start.y; (step == 1 && y <= End.y) || (step == -1 && y >= End.y); y += step)
		{
			SetPixelColor(Vec2d((int)x, y), Color);
			x += m;
		}
	}
}

//Implement Bresenham's line drawing
void Pixel::Bresenham(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3d& Color)
{
	if(abs(dy) < abs(dx))
	{
		int D = 0;
		int y = Start.y;
		int step = Start.x > End.x? -1: 1;
		for(int x = Start.x; x != End.x; x += step)
		{
			SetPixelColor(Vec2d(x, y), Color);
			if(D >= 0)
			{
				y++;
				D -= abs(dx);
			}
			D += dy;
		}
	}	
	else
	{
		int D = 0;
		int x = Start.x;
		int step = Start.y > End.y? -1: 1;
		for(int y = Start.y; y != End.y; y += step)
		{
			SetPixelColor(Vec2d(x, y), Color);
			if(D >= 0)
			{
				x++;
				D -= abs(dy);
			}
			D += dx;
		}
	}
}

//Line pre-processing before calling algorihtm
void Pixel::DrawLine(Vec2d Start, Vec2d End, const Vec3d& Color, bool Algorithm)
{	
	if(!CohenSutherland(Start, End))
		return;

	int dx = End.x - Start.x;
	int dy = End.y - Start.y;

	if(dx == 0)
		Vertical(Start, End, dx, dy, Color);
	else if(dy == 0)
		Horizontal(Start, End, dx, dy, Color);
	else if(abs(dy) == abs(dx))
		Straight(Start, End, dx, dy, Color);
	else
	{
		Swap(Start, End, dx, dy);

		if(Algorithm == ALGO_DDA)
			DDA(Start, End, dx, dy, Color);
		else
			Bresenham(Start, End, dx, dy, Color);
	}
}

//Bresenham's circle drawing algorihtm
void Pixel::DrawCircle(const Vec2d& Center, int Radius, const Vec3d& Color)
{
	int x = Radius;
	int y = 0;
	int err = 0;

	while(x >= y)
	{
		SetPixelColor(Vec2d(Center.x + x, Center.y + y), Color);
		SetPixelColor(Vec2d(Center.x + y, Center.y + x), Color);
		SetPixelColor(Vec2d(Center.x - y, Center.y + x), Color);
		SetPixelColor(Vec2d(Center.x - x, Center.y + y), Color);
		SetPixelColor(Vec2d(Center.x - x, Center.y - y), Color);
		SetPixelColor(Vec2d(Center.x - y, Center.y - x), Color);
		SetPixelColor(Vec2d(Center.x + y, Center.y - x), Color);
		SetPixelColor(Vec2d(Center.x + x, Center.y - y), Color);

		y++;
		err += 1 + 2 * y;
		if(2 * (err - x) + 1 > 0)
		{
			x--;
			err += 1 - 2 * x;
		}
	}
}

//Draw lines based on vertices and edges
void Pixel::DrawVertex(const std::vector<Vec2f>& Vertex, const std::vector<Vec2d>& Edge, const Vec3d& Color)
{
	Lock();

	//Screen size
	Vec2f Bound(Size.x - 1, Size.y - 1);

	//Calculate bounding box
	Vec2f d = MaxWorldCoor - MinWorldCoor;
	float factor = max(d.x, d.y);
	Vec2f Factor(factor, factor);

	//Normalize world coordinates
	for(unsigned int i = 0; i < Edge.size(); i++)
		DrawLine((Vertex[Edge[i].x] -  MinWorldCoor) * Bound / Factor, (Vertex[Edge[i].y] - MinWorldCoor) * Bound / Factor, Color);
	Unlock();
}

//Draw hardcoded digits
void Pixel::DrawNumber(const Vec2d& Position, int Value, const Vec3d& Color)
{
	char number[100];

	sprintf(number, "%d", Value);

	int num = strlen(number);

	int offset = Position.x - num * 7 / 2;

	for(int n = 0; n < num; n++)
		for(int i = 0; i < 7; i++)
			for(int j = 0; j < 5; j++)
				if(Digit[number[n] - '0'][i][j])
					SetPixelColor(Vec2d(offset + n * 7 + j,  Position.y + (6 - i) - 3), Color);
}

void Pixel::DrawLetter(const Vec2d& Position, const char* String, const Vec3d& Color)
{
	int num = strlen(String);

	int offset = Position.x - num * 7 / 2;

	for(int n = 0; n < num; n++)
		for(int i = 0; i < 7; i++)
			for(int j = 0; j < 5; j++)
				if(Character[String[n] - 'X'][i][j])
					SetPixelColor(Vec2d(offset + n * 7 + j,  Position.y + (6 - i) - 3), Color);
}

//Scan line rasterization
void Pixel::Rasterize(const std::vector<Vec3f>& Vertex, const Vec3d& Color)
{/*
	if(Vertex.size() == 0)
	return;

	std::vector<std::list<Vec2d> > Intersect;
	for(int y = Position.y; y < Position.y + Size.y; y++)
	{
	//New line
	Intersect.push_back(std::list<Vec2d>());
	for(unsigned int n = 0; n < Vertex.size(); n++)
	{
	Vec2d v = Vertex[n];
	Vec2d v1 = Vertex[(n + 1) % Vertex.size()];
	Vec2d v2 = n == 0? Vertex.back(): Vertex[(n - 1) % Vertex.size()];

	int x;

	//Scan line is outside the line segment
	if((y < v.y && y < v1.y) || (y > v.y && y > v1.y))
	continue;

	//Wait for the next iteration to check vertex
	if(y == v1.y)
	continue;

	//Vertex intersection
	if(y == v.y)
	{
	//Horizontal line
	if(v.y == v1.y || v.y == v2.y)
	continue;

	//Local extrema along y-axis
	if((v.y < v1.y && v.y < v2.y) || (v.y > v1.y && v.y > v2.y))
	continue;

	x = v.x;
	}
	else
	{
	//Vertical line
	if(v1.x == v.x)
	x = v1.x;
	//Regular slanted line
	else
	{
	//Compute line equation
	float m = ((float)(v1.y - v.y)) / (v1.x - v.x);
	float c = v1.y - m * v1.x;
	x = round((y - c) / m);

	//Check if the x lies on the line segment
	if((x < v.x && x < v1.x) || (x > v.x && x > v1.x))
	continue;
	}
	}

	//Insert coordinate
	std::list<Vec2d>::iterator it;
	for(it = Intersect.back().begin(); it != Intersect.back().end(); it++)
	{
	//if(x == it->x)
	//break;
	if(x < it->x)
	{
	Intersect.back().insert(it, Vec2d(x, y));
	break;
	}		
	}
	if(it == Intersect.back().end())
	Intersect.back().push_back(Vec2d(x, y));
}
}

//Draw coordinates
for(unsigned int i = 0; i < Intersect.size(); i++)
{
	for(std::list<Vec2d>::iterator it = Intersect[i].begin(); distance(it, Intersect[i].end()) > 1 && it != Intersect[i].end(); it++)
	{
		std::list<Vec2d>::iterator it1 = it;
		it1++;
		DrawLine(*(it) - Position, *(it1) - Position, Color);
		//cout << it->x << " " << it->y << " " << it1->x << " " << it1->y << "\t";
		it++;
	}
	//cout << endl;
}*/
}

int Pixel::Code(const Vec2d& Point)
{
	int code = IN;
	if(Point.x < ClipMin.x)
		code |= LEFT;
	else if(Point.x > ClipMax.x)
		code |= RIGHT;
	if(Point.y < ClipMin.y)
		code |= DOWN;
	else if(Point.y > ClipMax.y)
		code |= UP;

	return code;
}

Vec2d Pixel::Intersect(const Vec2d& Start, const Vec2d& End, const Vec2d& Min, const Vec2d& Max, int CheckCode)
{
	Vec2d point;
	if(CheckCode & UP)
	{
		point.x = Start.x + (End.x - Start.x) * (Max.y - Start.y) / (End.y - Start.y);
		point.y = Max.y;
	}
	else if(CheckCode & DOWN)
	{
		point.x = Start.x + (End.x - Start.x) * (Min.y - Start.y) / (End.y - Start.y);
		point.y = Min.y;
	}
	else if(CheckCode & RIGHT)
	{
		point.y = Start.y + (End.y - Start.y) * (Max.x - Start.x) / (End.x - Start.x);
		point.x = Max.x;
	}
	else if(CheckCode & LEFT)
	{
		point.y = Start.y + (End.y - Start.y) * (Min.x - Start.x) / (End.x - Start.x);
		point.x = Min.x;
	}

	return point;
}

//Cohen-Sutherland algorithm
bool Pixel::CohenSutherland(Vec2d& Start, Vec2d& End)
{
	if(ClipMin == Vec2d(-1, -1) || ClipMax == Vec2d(-1, -1))
		return true;

	bool accept = false;
	int code1 = Code(Start), code2 = Code(End);

	while(true)
	{
		if(!(code1 | code2))
			return true;	
		else if(code1 & code2)
			return false;
		else
		{
			int code = code1? code1: code2;

			Vec2d point = Intersect(Start, End, ClipMin, ClipMax, code);

			if(code == code1)
			{
				Start.x = point.x;
				Start.y = point.y;
				code1 = Code(Start);
			}
			else
			{
				End.x = point.x;
				End.y = point.y;
				code2 = Code(End);
			}
		}
	}
	return accept;
}

void Pixel::Clip(const Vec2d& Min, const Vec2d& Max)
{
	if(Max.x < Min.x || Max.y < Min.y)
	{
		ClipMin = ClipMax = Vec2d(-1, -1);
		return;
	}

	ClipMin = Min - Position;
	ClipMax = Max - Position;
}

void Pixel::Unclip()
{
	ClipMin = ClipMax = Vec2d(-1, -1);
}

void Pixel::Render()
{
	glDrawPixels(Size.x, Size.y, GL_RGB, GL_FLOAT, Buffer);
	if(Erase)
		Clear();	
}

void Pixel::Clear()
{
	memset(Buffer, 0, Size.x * Size.y * 3 * sizeof(float));
}

bool Pixel::Init(const Vec2d& Size, bool EraseState, PROJECTION Projection)
{
	SetPixelSize(Size);
	Erase = EraseState;
	ProjectionPlane = Projection;
	return true;
}
