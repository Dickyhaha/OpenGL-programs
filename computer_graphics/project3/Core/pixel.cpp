#include "pixel.h"

Pixel hub;

Pixel::Pixel(): Buffer(NULL), Size(Vec2d())
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

Vec3f Pixel::GetPixelColor(const Vec2d& PixelPosition) const
{
	if(PixelPosition.x < 0 || PixelPosition.x >= Size.x || PixelPosition.y < 0 || PixelPosition.y >= Size.y)
		return Vec3f();

	int position = (PixelPosition.y * Size.x + PixelPosition.x) * 3;
	return Vec3f(Buffer[position], Buffer[position + 1], Buffer[position + 2]);
}

//Actual modification of the pixel buffer
bool Pixel::SetPixelColor(const Vec2d& PixelPosition, const Vec3f& Color)
{	
	if(PixelPosition.x < 0 || PixelPosition.x >= Size.x || PixelPosition.y < 0 || PixelPosition.y >= Size.y)
		return false;

	if(HalfTone)
	{
		Vec2d Position = PixelPosition * 3;
		int Intensity = (int)(Color.sum() / 3 * 9);
		for(int i = 0; i < Intensity; i++)
		{
			int position = ((Position.y + i / 3) * DefaultSize.x + (Position.x + i % 3)) * 3;
			Buffer[position	   ] = Buffer[position + 1] = Buffer[position + 2] = 1.0f;
		}
	}
	else
	{
		int position = (PixelPosition.y * Size.x + PixelPosition.x) * 3;
		Buffer[position    ] = Color.x;
		Buffer[position + 1] = Color.y;
		Buffer[position + 2] = Color.z;
	}

	return true;
}

void Pixel::Swap(Vec2d& Start, Vec2d& End, Vec3f& StartColor, Vec3f& EndColor, int& dx, int& dy)
{
	if(abs(dy) > abs(dx) && Start.x < End.x)
		return;
	else if(abs(dy) <= abs(dx) && Start.y < End.y)
		return;

	Vec2d Temp = Start;
	Start = End;
	End = Temp;

	StartColor.swap(EndColor);

	dy = End.y - Start.y;
	dx = End.x - Start.x;
}

//If line is horizontal, no algorithm is needed
void Pixel::Horizontal(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor, const Vec3f& EndColor)
{
	Vec3f Color = StartColor;
	Vec3f step_color = (EndColor - StartColor) / (End - Start).distance();
	int step = Start.x > End.x? -1: 1;
	for(int x = Start.x; (step == 1 && x <= End.x) || (step == -1 && x >= End.x); x += step)
	{
		SetPixelColor(Vec2d(x, Start.y), Color);
		Color += step_color;
	}
}

//If line is vertical, no algorithm is needed
void Pixel::Vertical(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor, const Vec3f& EndColor)
{
	Vec3f Color = StartColor;
	Vec3f step_color = (EndColor - StartColor) / (End - Start).distance();
	int step = Start.y > End.y? -1: 1;
	for(int y = Start.y; (step == 1 && y <= End.y) || (step == -1 && y >= End.y); y += step)
	{
		SetPixelColor(Vec2d(Start.x, y), Color);
		Color += step_color;
	}
}

//If line is straight, no algorithm is needed
void Pixel::Straight(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor, const Vec3f& EndColor)
{
	Vec3f Color = StartColor;
	Vec3f step_color = (EndColor - StartColor) / (End - Start).distance();
	int stepx = Start.x > End.x? -1: 1;
	int stepy = Start.y > End.y? -1: 1;
	for(int x = Start.x, y = Start.y; (stepx == 1 && x <= End.x) || (stepx == -1 && x >= End.x); x += stepx, y += stepy)
	{
		SetPixelColor(Vec2d(x, y), Color);
		Color += step_color;
	}
}

//Implement DDA line drawing
void Pixel::DDA(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor, const Vec3f& EndColor)
{	
	float m = fabs(((float)dy)/dx);
	if(m <= 1)
	{
		float y = Start.y;
		int step = Start.x > End.x? -1: 1;
		for(int x = Start.x; (step == 1 && x <= End.x) || (step == -1 && x >= End.x); x += step)
		{
			SetPixelColor(Vec2d(x, (int)y), StartColor);
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
			SetPixelColor(Vec2d((int)x, y), StartColor);
			x += m;
		}
	}
}

//Implement Bresenham's line drawing
void Pixel::Bresenham(const Vec2d& Start, const Vec2d& End, int dx, int dy, const Vec3f& StartColor, const Vec3f& EndColor)
{
	Vec3f Color = StartColor;
	Vec3f step_color = (EndColor - StartColor) / (Start - End).distance();
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
			Color = Color + step_color;
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
			Color = Color + step_color;
		}
	}
}

//Line pre-processing before calling algorihtm
void Pixel::DrawLine(Vec2d Start, Vec2d End, Vec3f StartColor, Vec3f EndColor, bool Algorithm)
{	
	if(Start < Vec2d() || End < Vec2d() || Start >= Size || End >= Size)
		return;

	int dx = End.x - Start.x;
	int dy = End.y - Start.y;

	if(dx == 0)
		Vertical(Start, End, dx, dy, StartColor, EndColor);
	else if(dy == 0)
		Horizontal(Start, End, dx, dy, StartColor, EndColor);
	else if(abs(dy) == abs(dx))
		Straight(Start, End, dx, dy, StartColor, EndColor);
	else
	{
		Swap(Start, End, StartColor, EndColor, dx, dy);

		if(Algorithm == ALGO_DDA)
			DDA(Start, End, dx, dy, StartColor, EndColor);
		else
			Bresenham(Start, End, dx, dy, StartColor, EndColor);
	}
}

//Bresenham's circle drawing algorihtm
void Pixel::DrawCircle(const Vec2d& Center, int Radius, const Vec3f& Color)
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
void Pixel::DrawVertex(const std::vector<Vec2f>& Vertex, const std::vector<Vec3d>& Face, const std::vector<bool>& FrontFace, const std::vector< std::vector<Vec3f> >& Color, const std::vector<int>& FaceColor, const std::vector<bool>& Sight)
{
	if(Vertex.size() == 0 || Face.size() == 0 || Color.size() == 0)
		return;

	Lock();

	//NDC
	Vec2f Factor = Vec2f(Size - 1);

	for(unsigned int f = 0; f < Face.size(); f++)
	{
		if(!FrontFace[f])
			continue;

		if(!(Sight[Face[f].x] && Sight[Face[f].y] && Sight[Face[f].z]))
			continue;

		Vec2d v1 = (Vertex[Face[f].x] * Factor).round();
		Vec2d v2 = (Vertex[Face[f].y] * Factor).round();
		Vec2d v3 = (Vertex[Face[f].z] * Factor).round();

		Vec3f color1 = Color[FaceColor[f]][0];
		Vec3f color2 = Color[FaceColor[f]][1];
		Vec3f color3 = Color[FaceColor[f]][2];

		DrawLine(v1, v2, color1, color2);
		DrawLine(v2, v3, color2, color3);
		DrawLine(v3, v1, color3, color1);
	}

	Unlock();
}

//Draw hardcoded digits
void Pixel::DrawNumber(const Vec2d& Position, int Value, const Vec3f& Color)
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

void Pixel::DrawLetter(const Vec2d& Position, const char* String, const Vec3f& Color)
{
	int num = strlen(String);

	int offset = Position.x - num * 7 / 2;

	for(int n = 0; n < num; n++)
		for(int i = 0; i < 7; i++)
			for(int j = 0; j < 5; j++)
				if(Character[String[n] - 'X'][i][j])
					SetPixelColor(Vec2d(offset + n * 7 + j,  Position.y + (6 - i) - 3), Color);
}

Vec2d Pixel::Interpolate(Vec2d Start, Vec2d End, int y)
{
	if(Start.x == End.x)
		return Vec2d(Start.x, y);

	float m = ((float)(Start.y - End.y)) / (Start.x - End.x);
	float c = Start.y - m * Start.x;
	int x = round((y - c) / m);
	return Vec2d(x, y);
}

Vec3f Pixel::InterpolateColor(Vec2d Start, Vec2d End, Vec3f StartColor, Vec3f EndColor, int y)
{
	return EndColor * (float)(y - Start.y) / (End.y - Start.y) + StartColor * (float)(End.y - y) / (End.y - Start.y);
}

//Scan line rasterization
void Pixel::Rasterize(const std::vector<Vec2f>& Vertex, const std::vector<Vec3d>& Face, const std::vector<bool>& FrontFace, const std::vector< std::vector<Vec3f> >& Color, const std::vector<int>& FaceColor, const std::vector<bool>& Sight)
{
	if(Face.size() == 0)
		return;

	Lock();

	//NDC
	Vec2f Factor = Vec2f(Size - 1);

	for(unsigned int f = 0; f < Face.size(); f++)
	{
		if(!FrontFace[f])
			continue;

		if(!(Sight[Face[f].x] && Sight[Face[f].y] && Sight[Face[f].z]))
			continue;

		Vec2d v1 = (Vertex[Face[f].x] * Factor).round();
		Vec2d v2 = (Vertex[Face[f].y] * Factor).round();
		Vec2d v3 = (Vertex[Face[f].z] * Factor).round();

		Vec3f color1 = Color[FaceColor[f]][0];
		Vec3f color2 = Color[FaceColor[f]][1];
		Vec3f color3 = Color[FaceColor[f]][2];

		//if(v1.y == v2.y && v2.y == v3.y)
			//continue;

		//v1.y <= v2.y <= v3.y
		if(v1.y > v2.y)
		{
			v1.swap(v2);
			color1.swap(color2);
		}
		if(v1.y > v3.y)
		{
			v1.swap(v3);
			color1.swap(color3);
		}
		if(v2.y > v3.y)
		{
			v2.swap(v3);
			color2.swap(color3);
		}

		for(int y = v1.y; y < v3.y; y++)
		{
			Vec2d other = Interpolate(v1, v3, y);
			Vec3f other_color = InterpolateColor(v1, v3, color1, color3, y);
			if(y == v2.y)
				DrawLine(v2, other, color2, other_color);
			else if(y < v2.y)
				DrawLine(Interpolate(v1, v2, y), other, InterpolateColor(v1, v2, color1, color2, y), other_color);
			else 
				DrawLine(Interpolate(v2, v3, y), other, InterpolateColor(v2, v3, color2, color3, y), other_color);
		}
	}

	Unlock();
}

int Pixel::Code(const Vec2d& Point)
{
	/*int code = IN;
	  if(Point.x < ClipMin.x)
	  code |= LEFT;
	  else if(Point.x > ClipMax.x)
	  code |= RIGHT;
	  if(Point.y < ClipMin.y)
	  code |= DOWN;
	  else if(Point.y > ClipMax.y)
	  code |= UP;

	  return code;*/
	return 1;
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
{/*
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
	return accept;*/
	return true;
}

void Pixel::Render()
{
	glDrawPixels(DefaultSize.x, DefaultSize.y, GL_RGB, GL_FLOAT, Buffer);
	Clear();	
}

void Pixel::SetHalfTone(bool HalfToneState)
{
	HalfTone = HalfToneState;
	if(HalfTone)
		Size = DefaultSize / 3;
	else
		Size = DefaultSize;
}

void Pixel::Clear()
{
	memset(Buffer, 0, DefaultSize.x * DefaultSize.y * 3 * sizeof(float));
}

bool Pixel::Init(const Vec2d& Size, bool HalfTone)
{
	DefaultSize = Size;
	SetPixelSize(Size);
	SetHalfTone(HalfTone);
	return true;
}
