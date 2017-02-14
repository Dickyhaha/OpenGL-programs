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
			canvas.DrawLine(Start, End, Vec3d(255, 255, 255), ALGO_BRESENHAM);
		}
		else if(strcmp(input, "dda") == 0)
		{
			canvas.SetErase(false);
			Vec2d Start, End;
			cin >> Start.x >> Start.y >> End.x >> End.y;
			canvas.DrawLine(Start, End, Vec3d(255, 255, 255), ALGO_DDA);
		}
		else if(strcmp(input, "circle") == 0)
		{
			Vec2d Center;
			int radius;
			cin >> Center.x >> Center.y >> radius;
			canvas.DrawCircle(Center, radius, Vec3d(255, 255, 255));
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

			std::vector<Vec3f> vertices;
			Vec3f vertex;
			for(int i = 0; i < vertex_count; i++)
			{
				cin >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}

			int edge_count = 0;
			cin >> edge_count;

			std::vector<Vec2d> edges;
			Vec2d edge;
			for(int i = 0; i < edge_count; i++)
			{
				cin >> edge.x >> edge.y;
				edges.push_back(edge);
			}

			polygon->SetVertex(vertices, edges);
		}
		else if(strcmp(input, "color") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			Vec3d Color;
			cin >> Color.x >> Color.y >> Color.z;
			polygon->SetColor(Color);
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

			Vec3f Offset;
			cin >> Offset.x >> Offset.y >> Offset.z;
			polygon->Translate(Offset);
		}
		else if(strcmp(input, "rotate") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			Vec3f Start, End;
			float angle;
			cin >> Start.x >> Start.y >> Start.z >> End.x >> End.y >> End.z >> angle;

			world.PushPolygon();

			std::vector<Vec3f> axis;
			std::vector<Vec2d> edge;
			axis.push_back(Start);
			axis.push_back(End);
			edge.push_back(Vec2d(0, 1));
			Polygon* Axis = world.GetPolygon();
			Axis->SetVertex(axis, edge);
			Axis->SetColor(Vec3d(255, 0, 0));

			cout << "Are you sure you want to rotate the polygon around the displayed axis? [Y/N]" << endl;

			cin >> input;
			if(strcmp(input, "Y") == 0 || strcmp(input, "y") == 0)
				polygon->Rotate(Start, End, angle);
			world.PopPolygon();
		}
		else if(strcmp(input, "scale") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			Vec3f factor;
			cin >> factor.x >> factor.y >> factor.z;
			polygon->Scale(factor);
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

			float SpinRate = polygon->GetSpinRate();
			const Vec3f& Point = polygon->GetSpinPoint();
			const Vec3f& Direction = polygon->GetSpinDirection();
			cout << "\nSpin Rate: " << SpinRate << endl;
			cout << "\nSpin Point:\n" << Point.x << " " << Point.y << " " << Point.z << endl;
			cout << "\nSpin Direction:\n" << Direction.x << " " << Direction.y << " " << Direction.z << endl;

			const Vec3d& Color = polygon->GetColor();
			cout << "\nColor:\n" << Color.x << " " << Color.y << " " << Color.z << endl;

			const Vec3f& Center = polygon->GetCenter();
			cout << "\nCenter:\n" << Center.x << " " << Center.y << " " << Center.z << endl;

			const std::vector<Vec3f>& vertex = polygon->GetVertex();
			cout << "\nVertex count: " << vertex.size() << endl;
			for(unsigned int i = 0; i <vertex.size(); i++)
				cout << round(vertex[i].x) << " " << round(vertex[i].y) << " " << round(vertex[i].z) << endl;

			const std::vector<Vec2d>& edge = polygon->GetEdge();
			cout << "\nEdge count: " << edge.size() << endl;
			for(unsigned int i = 0; i < edge.size(); i++)
				cout << edge[i].x << " " << edge[i].y << endl;
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
			Vec3f point, direction;

			cin >> point.x >> point.y >> point.z >> direction.x >> direction.y >> direction.z >> rate;

			world.PushPolygon();

			std::vector<Vec3f> axis;
			std::vector<Vec2d> edge;
			axis.push_back(point);
			axis.push_back(direction);
			edge.push_back(Vec2d(0, 1));
			Polygon* Axis = world.GetPolygon();
			Axis->SetVertex(axis, edge);
			Axis->SetColor(Vec3d(255, 0, 0));

			cout << "Are you sure you want to spin the polygon around the displayed axis? [Y/N]" << endl;

			cin >> input;
			if(strcmp(input, "Y") == 0 || strcmp(input, "y") == 0)
				polygon->SetSpin(point, direction, rate);
			world.PopPolygon();
		}
		else if(strcmp(input, "stop") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			polygon->SetSpin(Vec3f(), Vec3f(), 0.0f);
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
