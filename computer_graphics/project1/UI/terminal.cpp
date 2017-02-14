#include "terminal.h"

using namespace std;

Terminal* terminal_ptr;

Terminal terminal;

Terminal::Terminal(): Running(false)
{
}

Terminal::~Terminal()
{
	Stop();
}

void Terminal::Run()
{
	char input[100];
	while(Running)
	{
		cout << "\n(Count: " << world.GetPolygonCount() << ", Selected: " << world.GetPolygonIndex() << ") Enter Command: ";
		fflush(stdout);
		cin >> input;
		if(strcmp(input, "bresenham") == 0)
		{
			canvas.SetErase(false);
			Vec2d Start, End;
			cin >> Start.x >> Start.y >> End.x >> End.y;
			canvas.DrawLine(Start, End, ALGO_BRESENHAM);
		}
		else if(strcmp(input, "dda") == 0)
		{
			canvas.SetErase(false);
			Vec2d Start, End;
			cin >> Start.x >> Start.y >> End.x >> End.y;
			canvas.DrawLine(Start, End, ALGO_DDA);
		}
		else if(strcmp(input, "circle") == 0)
		{
			Vec2d Center;
			int radius;
			cin >> Center.x >> Center.y >> radius;
			canvas.DrawCircle(Center, radius);
		}
		else if(strcmp(input, "set") == 0)
		{
			int Index;
			cin >> Index;
			world.SetPolygonIndex(Index);
		}
		else if(strcmp(input, "polygon") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
			{
				world.PushPolygon();
				polygon = world.GetPolygon();
			}

			int vertex_count = 0;
			cin >> vertex_count;

			std::vector<Vec2f> vertices;
			Vec2f vertex;

			for(int i = 0; i < vertex_count; i++)
			{
				cin >> vertex.x >> vertex.y;
				vertices.push_back(vertex);
			}
			polygon->SetVertex(vertices);
		}
		else if(strcmp(input, "push") == 0)
		{
			world.PushPolygon();
		}
		else if(strcmp(input, "pop") == 0)
		{
			world.PopPolygon();
		}
		else if(strcmp(input, "translate") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			Vec2d Offset;
			cin >> Offset.x >> Offset.y;
			polygon->Translate(Offset);
		}
		else if(strcmp(input, "rotate") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			float angle;
			cin >> angle;
			polygon->Rotate(angle);
		}
		else if(strcmp(input, "scale") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			float factorx, factory;
			cin >> factorx >> factory;
			polygon->Scale(factorx, factory);
		}
		else if(strcmp(input, "rasterize") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			polygon->SetRasterize(true);
		}
		else if(strcmp(input, "unrasterize") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			polygon->SetRasterize(false);
		}
		else if(strcmp(input, "cut") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			Vec2d Min, Max;
			cin >> Min.x >> Min.y >> Max.x >> Max.y;
			polygon->SutherlandHodgeman(Min, Max);
		}
		else if(strcmp(input, "info") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			const std::vector<Vec2f>& vertex = polygon->GetVertex();

			cout << "\nVertex count: " << vertex.size() << endl;
			for(unsigned int i = 0; i <vertex.size(); i++)
				cout << round(vertex[i].x) << " " << round(vertex[i].y) << endl;
		}
		else if(strcmp(input, "cls") == 0)
		{
			for(int i = 0; i < 50; i++)
				cout << endl;
		}	
		else if(strcmp(input, "swap") == 0)
		{
			int first, second;
			cin >> first >> second;
			world.Swap(first, second);
		}
		else if(strcmp(input, "spin") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			float rate;
			cin >> rate;
			polygon->Spinning(rate);
		}
		else if(strcmp(input, "stop") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			polygon->Spinning(0.0f);
		}
		else if(strcmp(input, "firework") == 0)
		{
			Vec2d Position;
			cin >> Position.x >> Position.y;

			canvas.DrawLine(Position, Vec2d(0, 1000) + Position);
			canvas.DrawLine(Position, Vec2d(500, 1000) + Position);
			canvas.DrawLine(Position, Vec2d(1000, 1000) + Position);
			canvas.DrawLine(Position, Vec2d(1000, 500) + Position);
			canvas.DrawLine(Position, Vec2d(1000, 0) + Position);
			canvas.DrawLine(Position, Vec2d(1000, -500) + Position);
			canvas.DrawLine(Position, Vec2d(1000, -1000) + Position);
			canvas.DrawLine(Position, Vec2d(500, -1000) + Position);
			canvas.DrawLine(Position, Vec2d(0, -1000) + Position);
			canvas.DrawLine(Position, Vec2d(-500, -1000) + Position);
			canvas.DrawLine(Position, Vec2d(-1000, -1000) + Position);
			canvas.DrawLine(Position, Vec2d(-1000, -500) + Position);
			canvas.DrawLine(Position, Vec2d(-1000, 0) + Position);
			canvas.DrawLine(Position, Vec2d(-1000, 500) + Position);
			canvas.DrawLine(Position, Vec2d(-1000, 1000) + Position);
			canvas.DrawLine(Position, Vec2d(-500, 1000) + Position);
		}	
		else if(strcmp(input, "clip") == 0)
		{
			Vec2d Min, Max;
			cin >> Min.x >> Min.y >> Max.x >> Max.y;
			canvas.Clip(Min, Max);
		}
		else if(strcmp(input, "unclip") == 0)
			canvas.Unclip();
		else if(strcmp(input, "load") == 0)
		{
			canvas.SetErase(true);
			world.LoadPolygon("polygons.txt");
		}
		else if(strcmp(input, "save") == 0)
			world.SavePolygon("polygons.txt");
		else if(strcmp(input, "erase") == 0)
			canvas.SetErase(true);
		else if(strcmp(input, "keep") == 0)
			canvas.SetErase(false);
		else if(strcmp(input, "flush") == 0)
			world.FlushPolygon();
		else if(strcmp(input, "clear") == 0)
			canvas.Clear();
	}
}

bool Terminal::Init()
{
	terminal_ptr = this;

	return true;
}

void Terminal::Start()
{
	if(!Running)
	{
		Running = true;
		Thread = pthread_create(&Thread, NULL, OnRun, NULL);
	}
}

void Terminal::Stop()
{
	if(Running)
		Running = false;
}

void* OnRun(void* ptr)
{
	if(terminal_ptr != NULL)
		terminal_ptr->Run();

	return NULL;
}
