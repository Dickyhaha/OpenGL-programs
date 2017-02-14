#include "renderer.h"

Renderer renderer;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

Vec2d Renderer::GetSize()
{
	return Canvas.GetPixelSize();
}

bool Renderer::Init(const Vec2d& Size, PROJECTION NewProjection)
{
	Projection = NewProjection;

	if(Projection == PROJECTION_XY)
	{
		FromVector = Vec3f(0.0f, 0.0f, -5.0f);
		UpVector = Vec3f(0.0f, 1.0f, 0.0f);
	}
	else if(Projection == PROJECTION_XZ)
	{
		FromVector = Vec3f(0.0f, 5.0f, 0.0f);
		UpVector = Vec3f(0.0f, 0.0f, 1.0f);
	}
	else if(Projection == PROJECTION_YZ)
	{
		FromVector = Vec3f(5.0f, 0.0f, 0.0f);
		UpVector = Vec3f(0.0f, 1.0f, 0.0f);
	}

	AtVector = Vec3f(0.0f, 0.0f, 0.0f);
	ViewAngle = M_PI / 6.0f;
	Zmin = -20.0f;
	Zmax = 20.0f;

	return Canvas.Init(Size);
}

Vec3f Renderer::GetFromVector()
{
	return FromVector;
}

void Renderer::SetFromVector(const Vec3f& NewFromVector)
{
	FromVector = NewFromVector;
}

void Renderer::SetHalfTone(bool State)
{
	Canvas.SetHalfTone(State);
}

void Renderer::LookAt(const Vec3f& From, const Vec3f& At, const Vec3f& Up, float Angle, float Min, float Max)
{
	FromVector = From;
	AtVector = At;
	UpVector = Up;

	ViewAngle = Angle;

	Zmin = Min;
	Zmax = Max;
}

//Camera Viewing model
void Renderer::CVM(const std::vector<Vec3f>& Vertex, std::vector<Vec3f>& VertexBuffer, std::vector<Vec2f>& ProjectionBuffer, std::vector<bool>& SightBuffer)
{
	Vec3f b1, b2, b3;
	b3 = (AtVector - FromVector).normalize();
	b1 = b3.cross(UpVector).normalize();
	b2 = b1.cross(b3);

	for(unsigned int v = 0; v < Vertex.size(); v++)
	{
		Vec3f vertex = Vertex[v] - FromVector;

		Vec3f point;
		point.x = (b1 * vertex).sum();
		point.y = (b2 * vertex).sum();
		point.z = (b3 * vertex).sum();

		VertexBuffer.push_back(point);

		float D = 1 / (2 * tan(ViewAngle)) / point.z;

		Vec2f projection;
		projection.x = point.x * D + 0.5f;
		projection.y = point.y * D + 0.5f;

		ProjectionBuffer.push_back(projection);

		SightBuffer.push_back(projection.x <= 1.0f && projection.x >= 0.0f && projection.y <= 1.0f && projection.y >= 0.0f);
	}
}

//Backface Culling
void Renderer::BFC(const std::vector<Vec3f>& Vertex, const std::vector<Vec3d>& Face, const std::vector<Vec3f>& Normal, const std::vector<Vec3d>& FaceNormal, std::vector<bool>& FrontFaceBuffer)
{
	for(unsigned int f = 0; f < Face.size(); f++)
	{
		Vec3f fn = (Vertex[Face[f].y] - Vertex[Face[f].x]).cross(Vertex[Face[f].z] - Vertex[Face[f].x]);
		fn = (fn * Normal[FaceNormal[f].x]).sum() < 0.0f? fn * (-1): fn;
		FrontFaceBuffer.push_back(((fn * (FromVector - Vertex[Face[f].x])).sum()) >= 0.0f);
	}
}

//Sort the faces of all polygons
void Renderer::Sort(const std::vector<Vec3f>& VertexBuffer, std::vector<Vec3d>& FaceBuffer, std::vector<int>& FaceColorBuffer, std::vector<bool>& FrontFaceBuffer)
{
	std::list<float> depth;
	std::list<Vec3d> index;
	std::list<bool>  fface;
	std::list<int>   cface;

	for(unsigned int f = 0; f < FaceBuffer.size(); f++)
	{
		float closest_depth = min(VertexBuffer[FaceBuffer[f].x].z, min(VertexBuffer[FaceBuffer[f].y].z, VertexBuffer[FaceBuffer[f].z].z));

		std::list<float>::iterator depth_it;
		std::list<Vec3d>::iterator index_it;
		std::list<bool> ::iterator fface_it;
		std::list<int>  ::iterator cface_it;
		for(depth_it = depth.begin(), index_it = index.begin(), cface_it = cface.begin(), fface_it = fface.begin();
				depth_it != depth.end();
				index_it++, depth_it++, cface_it++, fface_it++)
		{
			if(closest_depth > *depth_it)
			{
				depth.insert(depth_it, closest_depth);
				index.insert(index_it, FaceBuffer[f]);
				fface.insert(fface_it, FrontFaceBuffer[f]);
				cface.insert(cface_it, f);
				break;
			}
		}

		if(depth_it == depth.end())
		{
			depth.push_back(closest_depth);
			index.push_back(FaceBuffer[f]);
			fface.push_back(FrontFaceBuffer[f]);
			cface.push_back(f);
		}
	}

	FaceBuffer 		= std::vector<Vec3d>(index.begin(), index.end());
	FaceColorBuffer = std::vector<int>	(cface.begin(), cface.end());
	FrontFaceBuffer = std::vector<bool> (fface.begin(), fface.end());
}

void Renderer::Draw(const std::vector<Polygon*>& PolygonStream, const std::vector<Light*>& LightStream, const std::vector<Curve*>& CurveStream)
{
	//Draw polygons
	if(PolygonStream.size() != 0 && LightStream.size() != 0)
	{
		//Overall value buffer, different size
		std::vector<Vec3f> VertexBuffer;
		std::vector<Vec2f> ProjectionBuffer;
		std::vector<bool> SightBuffer;
		//Overall index buffer, same size
		std::vector<Vec3d> FaceBuffer;
		std::vector<bool> FrontFaceBuffer;
		//Color buffer for each vertex of each face
		std::vector< std::vector<Vec3f> > ColorBuffer;
		std::vector<int> FaceColorBuffer;

		//Load vertices and normals to local buffer, calculate light color
		for(unsigned int p = 0; p < PolygonStream.size(); p++)
		{
			const std::vector<Vec3f> vertex = PolygonStream[p]->GetVertex();
			const std::vector<Vec3f> normal = PolygonStream[p]->GetNormal();
			const std::vector<Vec3d> face = PolygonStream[p]->GetFace();
			const std::vector<Vec3d> face_normal = PolygonStream[p]->GetFaceNormal();

			//Temporary, currently per polygon color, will change to per face color
			Vec3f diffuse_color = PolygonStream[p]->GetDiffuseColor();
			Vec3f specular_color = PolygonStream[p]->GetSpecularColor();

			for(unsigned int f = 0; f < face.size(); f++)
			{
				//Push to local face buffer
				FaceBuffer.push_back(face[f] + VertexBuffer.size());

				//For each light souce
				std::vector<Vec3f> color(3);
				for(unsigned int l = 0; l < LightStream.size(); l++)
				{
					//Vertex colors of the 3 vertices of a face
					color[0] += LightStream[l]->TrueColor(vertex[face[f].x], normal[face_normal[f].x], FromVector, diffuse_color, specular_color);
					color[1] += LightStream[l]->TrueColor(vertex[face[f].y], normal[face_normal[f].y], FromVector, diffuse_color, specular_color);
					color[2] += LightStream[l]->TrueColor(vertex[face[f].z], normal[face_normal[f].z], FromVector, diffuse_color, specular_color);
				}
				ColorBuffer.push_back(color);
			}

			//CVM	
			CVM(vertex, VertexBuffer, ProjectionBuffer, SightBuffer);

			//Backface Culling
			BFC(vertex, face, normal, face_normal, FrontFaceBuffer);
		}

		//Painter's algorithm
		Sort(VertexBuffer, FaceBuffer, FaceColorBuffer, FrontFaceBuffer);

		//Pass to canvas for vertex drawing and rasterization
		Canvas.DrawVertex(ProjectionBuffer, FaceBuffer, FrontFaceBuffer, ColorBuffer, FaceColorBuffer, SightBuffer);
		Canvas.Rasterize(ProjectionBuffer, FaceBuffer, FrontFaceBuffer, ColorBuffer, FaceColorBuffer, SightBuffer);
	}

	//Draw curves
	if(CurveStream.size() != 0)
	{
		//Stores all the points
		std::vector< std::vector<Vec2f> > Point;
		std::vector< std::vector<Vec2f> > Control;
		std::vector< Vec3f > Color;
		for(unsigned int c = 0; c < CurveStream.size(); c++)
		{
			Control.push_back(CurveStream[c]->GetControlPoint());
			if(Control[c].size() == 0)
				continue;

			Color.push_back(CurveStream[c]->GetColor());

			CURVE CurveType = CurveStream[c]->GetCurveType();

			int Segment = CurveStream[c]->GetSegment();

			std::vector< std::vector<Vec2f> > B;

			if(CurveType == BEZIER)
			{
				float Step = 1.0f / Segment;
				Point.push_back(std::vector<Vec2f>());
				//deCasteljau's
				for(float t = 0.0f; t <= 1.0f; t += Step)
				{	
					B.push_back(Control[c]);
					for(unsigned int i = 1; i < Control[c].size(); i++)
					{
						B.push_back(std::vector<Vec2f>());
						for(unsigned int j = 0; j < Control[c].size() - i; j++)
							B[i].push_back(B[i - 1][j] * (1 - t) + B[i - 1][j + 1] * t);
					}
					Point[c].push_back(B.back().back());
					B.clear();
				}
			}
			else
			{
				//deBoor's
				unsigned int k = CurveStream[c]->GetDegree();
				unsigned int n = Control[c].size();
				const std::vector<float> u = CurveStream[c]->GetKnot();
				if(u.size() == 0)
					continue;
				
				float Step = (u[Control[c].size()] - u[k - 1]) / Segment;
				Point.push_back(std::vector<Vec2f>());
				for(float t = u[k - 1]; t <= u[n]; t += Step)
				{
					unsigned int I;
					for(I = 0; t >= u[I + 1] && I < u.size() - 1; I++);
					
					B.push_back(std::vector<Vec2f>());
					for(unsigned int d = I - (k - 1); d <= I; d++)
						B[0].push_back(Control[c][d]);
					
					for(unsigned int j = 1; j <= k - 1; j++)
					{
						B.push_back(std::vector<Vec2f>());
						for(unsigned int i = I - (k - 1); i <= I - j; i++)
						{
							unsigned int offset = i - (I - (k - 1));
							B[j].push_back(B[j - 1][offset] * (u[i + k] - t) / (u[i + k] - u[i + j]) + B[j - 1][offset + 1] * (t - u[i + j]) / (u[i + k] - u[i + j]));
						}
					}
					Point[c].push_back(B.back().back());
					B.clear();
				}
			}
		}

		if(Point.size() == 0 || Point[0].size() == 0)
			return;

		//Normalize to [0,1]
		Vec2f Min = Point[0][0], Max = Point[0][0];
		for(unsigned int c = 0; c < Point.size(); c++)
		{
			for(unsigned int p = 0; p < Point[c].size(); p++)
			{
				if(Point[c][p].x < Min.x)
					Min.x = Point[c][p].x;
				else if(Point[c][p].x > Max.x)
					Max.x = Point[c][p].x;
				if(Point[c][p].y < Min.y)
					Min.y = Point[c][p].y;
				else if(Point[c][p].y > Max.y)
					Max.y = Point[c][p].y;
			}
		}
		int Index = world.GetCurveIndex();
		for(unsigned int p = 0; p < Control[Index].size(); p++)
		{
			if(Control[Index][p].x < Min.x)
				Min.x = Control[Index][p].x;
			else if(Control[Index][p].x > Max.x)
				Max.x = Control[Index][p].x;
			if(Control[Index][p].y < Min.y)
				Min.y = Control[Index][p].y;
			else if(Control[Index][p].y > Max.y)
				Max.y = Control[Index][p].y;
		}
		if(world.GetManipulate())
		{
			for(unsigned int p = 0; p < Control[Index].size(); p++)
			{
				Control[Index][p] = (Control[Index][p] - Min) / (Max - Min);
				Canvas.DrawCircle(Control[Index][p], 10, RED, true);
				Canvas.DrawNumber(Control[Index][p], p, RED, true);
			}
			for(int p = 0; p < (int)(Control[Index].size()) - 1; p++)
				Canvas.DrawLine(Control[Index][p], Control[Index][p + 1], GREY, GREY, true);
		}
		for(unsigned int c = 0; c < Point.size(); c++)
		{
			for(unsigned int p = 0; p < Point[c].size(); p++)
				Point[c][p] = (Point[c][p] - Min) / (Max - Min);
			Canvas.DrawCurve(Point[c], Color[c]);
		}	
	}
	Render();
}

void Renderer::Render()
{
	Canvas.Render();
}
