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
		cout <<
			"\n(Polygon: " << world.GetPolygonIndex() + 1 << "/" << world.GetPolygonCount() <<
			", Light: " << world.GetLightIndex() + 1 << "/" << world.GetLightCount() << 
			", Curve: " << world.GetCurveIndex() + 1 << "/" << world.GetCurveCount() <<
			") Enter Command: ";
		cin.clear();
		cin >> input;
		if(strcmp(input, "set") == 0)
		{
			char type[10];
			cin >> type;

			int Index;
			cin >> Index;

			if(strcmp(type, "polygon") == 0)
				world.SetPolygonIndex(Index - 1);
			else if(strcmp(type, "light") == 0)
				world.SetLightIndex(Index - 1);
			else if(strcmp(type, "curve") == 0)
				world.SetCurveIndex(Index - 1);
		}
		else if(strcmp(input, "obj") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			world.Lock();

			char file_name[100];
			cin >> file_name;
			polygon->LoadModel(file_name);

			world.Unlock();
		}
		else if(strcmp(input, "color") == 0)
		{
			char type[10];
			cin >> type;

			if(strcmp(type, "polygon") == 0)
			{
				Polygon* polygon = world.GetPolygon();
				if(polygon == NULL)
					continue;

				cin >> type;

				Vec3f Color;
				cin >> Color.x >> Color.y >> Color.z;
				Color = Color / 255.0f;

				if(strcmp(type, "specular") == 0)
					polygon->SetSpecularColor(Color);
				else if(strcmp(type, "diffuse") == 0)
					polygon->SetDiffuseColor(Color);
			}
			else if(strcmp(type, "curve") == 0)
			{
				Curve* curve = world.GetCurve();
				if(curve == NULL)
					continue;

				Vec3f Color;
				cin >> Color.x >> Color.y >> Color.z;
				Color = Color / 255.0f;

				curve->SetColor(Color);
			}
		}
		else if(strcmp(input, "intensity") == 0)
		{
			Light* light = world.GetLight();
			if(light == NULL)
				continue;

			Vec3f Color;
			cin >> Color.x >> Color.y >> Color.z;

			light->SetIntensity(Color);
		}
		else if(strcmp(input, "model") == 0)
		{
			Light* light = world.GetLight();
			if(light == NULL)
				continue;

			char type[10];
			cin >> type;

			if(strcmp(type, "diffuse") == 0)
				light->SetLightModel(DIFFUSE);	
			else if(strcmp(type, "ambient") == 0)
				light->SetLightModel(AMBIENT);
		}
		else if(strcmp(input, "power") == 0)
		{
			Light* light = world.GetLight();
			if(light == NULL)
				continue;

			int power;
			cin >> power;

			light->SetPower(power);
		}
		else if(strcmp(input, "push") == 0)
		{
			char type[10];
			cin >> type;

			if(strcmp(type, "polygon") == 0)
				world.PushPolygon();
			else if(strcmp(type, "light") == 0)
				world.PushLight();
			else if(strcmp(type, "curve") == 0)
				world.PushCurve();
		}
		else if(strcmp(input, "pop") == 0)
		{
			char type[10];
			cin >> type;

			if(strcmp(type, "polygon") == 0)
				world.PopPolygon();
			else if(strcmp(type, "light") == 0)
				world.PopLight();
			else if(strcmp(type, "curve") == 0)
				world.PopCurve();
		}
		else if(strcmp(input, "translate") == 0)
		{		
			char type[10];
			cin >> type;

			if(strcmp(type, "polygon") == 0)
			{
				Polygon* polygon = world.GetPolygon();
				if(polygon == NULL)
					continue;

				Vec3f Offset;
				cin >> Offset.x >> Offset.y >> Offset.z;
				polygon->Translate(Offset);
			}
			else if(strcmp(type, "curve") == 0)
			{
				Curve* curve = world.GetCurve();
				if(curve == NULL)
					continue;

				Vec2f Offset;
				cin >> Offset.x >> Offset.y;
				curve->Translate(Offset);
			}
		}
		else if(strcmp(input, "rotate") == 0)
		{
			char type[10];
			cin >> type;

			if(strcmp(type, "polygon") == 0)
			{
				Polygon* polygon = world.GetPolygon();
				if(polygon == NULL)
					continue;

				Vec3f Start, End;
				float angle;
				cin >> Start.x >> Start.y >> Start.z >> End.x >> End.y >> End.z >> angle;
				polygon->Rotate(Start, End, angle * M_PI / 180.0f);
			}
			else if(strcmp(type, "curve") == 0)
			{
				Curve* curve = world.GetCurve();
				if(curve == NULL)
					continue;

				float angle;
				cin >> angle;
				curve->Rotate(angle * M_PI / 180.0f);
			}
		}
		else if(strcmp(input, "scale") == 0)
		{	
			char type[10];
			cin >> type;

			if(strcmp(type, "polygon") == 0)
			{
				Polygon* polygon = world.GetPolygon();
				if(polygon == NULL)
					continue;

				Vec3f factor;
				cin >> factor.x >> factor.y >> factor.z;
				polygon->Scale(factor);
			}
			else if(strcmp(type, "curve") == 0)
			{
				Curve* curve = world.GetCurve();
				if(curve == NULL)
					continue;

				Vec2f Factor;
				cin >> Factor.x >> Factor.y;
				curve->Scale(Factor);
			}
		}
		else if(strcmp(input, "position") == 0)
		{
			Light* light = world.GetLight();
			if(light == NULL)
				continue;

			Vec3f Position;
			cin >> Position.x >> Position.y >> Position.z;
			light->SetPosition(Position);
		}	
		else if(strcmp(input, "direction") == 0)
		{
			Light* light = world.GetLight();
			if(light == NULL)
				continue;

			Vec3f Position;
			cin >> Position.x >> Position.y >> Position.z;
			light->SetDirection(Position);
		}
		else if(strcmp(input, "curve") == 0)
		{
			Curve* curve = world.GetCurve();
			if(curve == NULL)
				continue;

			char CurveType[10];
			cin >> CurveType;
			int Degree;

			if(strcmp(CurveType, "bezier") == 0)
				curve->SetCurveType(BEZIER);
			else if(strcmp(CurveType, "bspline") == 0)
			{
				curve->SetCurveType(BSPLINE);
				cin >> Degree;
			}
			else 
				continue;

			int PointCount;
			std::vector<Vec2f> ControlPoint;
			Vec2f Point;
			int Segment;

			cin >> Segment >> PointCount;

			if(strcmp(CurveType, "bspline") == 0 && PointCount <= Degree)
				continue;

			for(int i = 0; i < PointCount; i++)
			{
				cin >> Point.x >> Point.y;
				ControlPoint.push_back(Point);
			}

			curve->SetColor(strcmp(CurveType, "bezier")? GREEN: CYAN);
			curve->SetSegment(Segment);
			curve->SetControlPoint(ControlPoint);
			curve->SetDegree(Degree);
		}
		else if(strcmp(input, "degree") == 0)
		{
			Curve* curve = world.GetCurve();
			if(curve == NULL)
				continue;

			unsigned int degree;
			cin >> degree;

			if(degree > curve->GetControlPoint().size())
				continue;

			curve->SetDegree(degree);
		}
		else if(strcmp(input, "knot") == 0)
		{
			Curve* curve = world.GetCurve();
			if(curve == NULL)
				continue;

			unsigned int degree;
			float knot;
			std::vector<float> knots;

			unsigned int point_count = curve->GetControlPoint().size();

			cin >> degree;
			if(degree > point_count)
				continue;

			for(unsigned int i = 0; i < degree + point_count; i++)
			{
				cin >> knot;
				knots.push_back(knot);
			}
			curve->SetKnot(knots);
		}
		else if(strcmp(input, "segment") == 0)
		{
			Curve* curve = world.GetCurve();
			if(curve == NULL)
				continue;

			int Segment;
			cin >> Segment;

			curve->SetSegment(Segment);
		}
		else if(strcmp(input, "move") == 0)
		{
			Curve* curve = world.GetCurve();
			if(curve == NULL)
				continue;

			int Index;
			cin >> Index;

			Vec2f Point;
			cin >> Point.x >> Point.y;
			curve->SetPoint(Index, Point);
		}
		else if(strcmp(input, "remove") == 0)
		{
			Curve* curve = world.GetCurve();
			if(curve == NULL)
				continue;

			int Index;
			cin >> Index;

			curve->RemovePoint(Index);
		}
		else if(strcmp(input, "add") == 0)
		{
			Curve* curve = world.GetCurve();
			if(curve == NULL)
				continue;

			int Index;
			cin >> Index;

			Vec2f Point;
			cin >> Point.x >> Point.y;

			curve->AddPoint(Index, Point);
		}
		else if(strcmp(input, "info") == 0)
		{
			Polygon* polygon = world.GetPolygon();

			Light* light = world.GetLight();

			Curve* curve = world.GetCurve();

			if(polygon != NULL)
			{
				cout << "\nPolygon\n----------\n" << endl;

				const Vec3f& Center = polygon->GetCenter();
				cout << "Center:\n" << Center  << endl;

				const Vec3f& Diffuse = polygon->GetDiffuseColor();
				cout << "Diffuse Color:\n" << Diffuse << endl;

				const Vec3f& Specular = polygon->GetSpecularColor();
				cout << "Specular Color:\n" << Specular << endl;
			}

			if(light != NULL)
			{
				cout << "\nLight\n----------\n" << endl;

				LIGHT Model = light->GetLightModel();
				cout << "Light Model: " << (Model == AMBIENT? "AMBIENT": "DIFFUSE") << "\n" << endl;

				if(Model == DIFFUSE)
				{
					const Vec3f& Position = light->GetPosition();
					cout << "Position:\n" << Position << endl;

					const Vec3f& Direction = light->GetDirection();
					cout << "Direction:\n" << Direction << endl;

					int Power = light->GetPower();
					cout << "Specular degree: " << Power << "\n" << endl;
				}

				const Vec3f& Color = light->GetIntensity();
				cout << "Intensity:\n" << Color << endl;
			}

			if(curve != NULL)
			{
				cout << "\nCurve\n----------\n" << endl;

				CURVE Type = curve->GetCurveType();
				cout << "Curve Type: " << (Type? "B-Spline": "Bezier") << "\n" << endl;

				if(Type == BSPLINE)
					cout << "Degree: " << curve->GetDegree() << "\n" << endl;;

				cout << "Segment: " << curve->GetSegment() << "\n" << endl;

				cout << "Color: " << curve->GetColor() * 255 << endl;

				const std::vector<Vec2f>& ControlPoint = curve->GetControlPoint();
				cout << "Control Point Count: " << ControlPoint.size() << endl;
				for(unsigned int p = 0; p < ControlPoint.size(); p++)
					cout << ControlPoint[p].x << " " << ControlPoint[p].y << endl;
				cout << endl;

				if(Type == BSPLINE)
				{
					const std::vector<float>& Knot = curve->GetKnot();
					cout << "Knot Count: " << Knot.size() << endl;
					for(unsigned int k = 0; k < Knot.size(); k++)
						cout << Knot[k] << " ";
					cout << endl;
				}
				cout << endl;
			}
		}
		else if(strcmp(input, "cls") == 0)
		{
			for(int i = 0; i < 50; i++)
				cout << endl;
		}	
		else if(strcmp(input, "swap") == 0)
		{
			char type[10];
			cin >> type;

			int first, second;
			cin >> first >> second;

			if(strcmp(type, "polygon") == 0)
				world.SwapPolygon(first - 1, second - 1);
			else if(strcmp(type, "light") == 0)
				world.SwapLight(first - 1, second - 1);
			else if(strcmp(type, "curve") == 0)
				world.SwapCurve(first - 1, second - 1);
		}
		else if(strcmp(input, "spin") == 0)
		{
			char type[10];
			cin >> type;

			if(strcmp(type, "polygon") == 0)
			{
				Polygon* polygon = world.GetPolygon();
				if(polygon == NULL)
					continue;

				float rate;
				Vec3f point, direction;

				cin >> point.x >> point.y >> point.z >> direction.x >> direction.y >> direction.z >> rate;
				polygon->SetSpin(point, direction, rate * M_PI / 180.0f);
			}
			else if(strcmp(type, "curve") == 0)
			{
				Curve* curve = world.GetCurve();
				if(curve == NULL)
					continue;

				float rate;
				cin >> rate;
				curve->SetSpinRate(rate * M_PI / 180.0f);
			}
		}
		else if(strcmp(input, "stop") == 0)
		{
			Polygon* polygon = world.GetPolygon();
			if(polygon == NULL)
				continue;

			polygon->SetSpin(Vec3f(), Vec3f(), 0.0f);
		}
		else if(strcmp(input, "load") == 0)
		{
			world.LoadPolygon("polygons.txt");
			world.LoadLight("lights.txt");
			world.LoadCurve("curves.txt");
		}
		else if(strcmp(input, "save") == 0)
		{
			world.SavePolygon("polygons.txt");
			world.SaveLight("lights.txt");
			world.SaveCurve("curves.txt");
		}
		else if(strcmp(input, "htone") == 0)
		{
			bool state;
			cin >> state;

			renderer.SetHalfTone(state);
		}
		else if(strcmp(input, "from") == 0)
		{
			Vec3f FromVector;

			cin >> FromVector.x >> FromVector.y >> FromVector.z;

			renderer.SetFromVector(FromVector);
		}
		else if(strcmp(input, "lookat") == 0)
		{
			Vec3f From, At, Up;
			float Angle, Zmin, Zmax;

			char mode[20];
			cin >> mode;

			if(strcmp(mode, "xy") == 0)
			{
				From = Vec3f(0.0f, 0.0f, -5.0f);
				Up = Vec3f(0.0f, 1.0f, 0.0f);
				At = Vec3f(0.0f, 0.0f, 0.0f);
				Angle = M_PI / 6.0f;
				Zmin = -20.0f;
				Zmax = 20.0f;
			}
			else if(strcmp(mode, "xz") == 0)
			{
				From = Vec3f(0.0f, 5.0f, 0.0f);
				Up = Vec3f(0.0f, 0.0f, 1.0f);
				At = Vec3f(0.0f, 0.0f, 0.0f);
				Angle = M_PI / 6.0f;
				Zmin = -20.0f;
				Zmax = 20.0f;
			}
			else if(strcmp(mode, "yz") == 0)
			{
				From = Vec3f(5.0f, 0.0f, 0.0f);
				Up = Vec3f(0.0f, 1.0f, 0.0f);
				At = Vec3f(0.0f, 0.0f, 0.0f);
				Angle = M_PI / 6.0f;
				Zmin = -20.0f;
				Zmax = 20.0f;
			}
			else
			{
				From.x = atof(mode);
				cin >> From.y >> From.z;
				cin >> At.x >> At.y >> At.z;
				cin >> Up.x >> Up.y >> Up.z;

				cin >> Angle >> Zmin >> Zmax;

				Angle = Angle * M_PI / 180.0f;
			}

			renderer.LookAt(From, At, Up, Angle, Zmin, Zmax);
		}
		else if(strcmp(input, "flush") == 0)
		{
			world.FlushPolygon();
			world.FlushLight();
		}
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
