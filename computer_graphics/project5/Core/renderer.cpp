#include "renderer.h"

Renderer renderer;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
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
	ViewAngle = M_PI / 4.0f;
	Zmin = 1.0f;
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

void Renderer::CVM(const Vec3f& Position, std::vector<Vec3f>& PositionBuffer)
{
	Vec3f b1, b2, b3;
	b3 = (AtVector - FromVector).normalize();
	b1 = b3.cross(UpVector).normalize();
	b2 = b1.cross(b3);

	Vec3f vertex = Position - FromVector;

	Vec3f point;
	point.x = (b1 * vertex).sum();
	point.y = (b2 * vertex).sum();
	point.z = (b3 * vertex).sum();

	PositionBuffer.push_back(point);
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

//Normalize color with max color value
void Renderer::NormalizeColor(std::vector<std::vector<Vec3f> >& ColorBuffer)
{
	if(ColorBuffer.size() == 0)
		return;

	float MaxColor = 0.0f;
	for(unsigned int p = 0; p < ColorBuffer.size(); p++)
	{
		for(unsigned int c = 0; c < ColorBuffer[p].size(); c++)
		{
			if(ColorBuffer[p][c].x > MaxColor)
				MaxColor = ColorBuffer[p][c].x;
			if(ColorBuffer[p][c].y > MaxColor)
				MaxColor = ColorBuffer[p][c].y;
			if(ColorBuffer[p][c].z > MaxColor)
				MaxColor = ColorBuffer[p][c].z;
		}
	}

	for(unsigned int p = 0; p < ColorBuffer.size(); p++)
		for(unsigned int c = 0; c < ColorBuffer[p].size(); c++)
			ColorBuffer[p][c] = ColorBuffer[p][c] / MaxColor;
}

float Renderer::Intersect(const Vec3f& Point, const Vec3f& Direction, const Vec3f& Position)
{
	float A = Direction.x * Direction.x + Direction.y * Direction.y + Direction.z * Direction.z;
	float B = 2 * (Point.x * Direction.x + Point.y * Direction.y + Point.z * Direction.z);
	float C = Point.x * Point.x + Point.y * Point.y + Point.z * Point.z;

	float Det = B * B - 4 * A * C;
	
	if(Det < 0.0f)
		return 0.0f;
	
	float t1 = (-B - sqrt(Det)) / (2 * A);
	float t2 = (-B + sqrt(Det)) / (2 * A);

	return t1 < 0.0f? (t2 < 0.0f? 0.0f: t2): t1;
}

Vec3f Renderer::Trace(const Vec3f& From, const Vec3f& Direction, const std::vector<Vec3f>& PositionBuffer, const std::vector<Vec3f>& DiffuseBuffer, const std::vector<Vec3f>& SpecularBuffer, const std::vector<Light*>& LightStream)
{
	int Index;
	float t = 0.0f;
	for(unsigned int p = 0; p < PositionBuffer.size(); p++)
	{
		float temp = Intersect(From, Direction, PositionBuffer[p]);
		if(temp > 0.0f && temp < t)
		{
			Index = p;
			t = temp;
		}
	}

	if(t == 0.0f)
		return Vec3f();

	Vec3f Point = From + Direction * t;
	Vec3f Normal = ((Point - PositionBuffer[Index]) * 2).normalize();

	Vec3f Color;
   	for(unsigned int l = 0; l < LightStream.size(); l++)
		Color += LightStream[l]->TrueColor(Point, Normal, From, DiffuseBuffer[Index], SpecularBuffer[Index]);

	return Color;
}

void Renderer::DrawPhong(const std::vector<Polygon*>& PolygonStream, const std::vector<Light*>& LightStream)
{
	if(PolygonStream.size() == 0 || LightStream.size() == 0)
		return;

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

	NormalizeColor(ColorBuffer);

	//Painter's algorithm
	Sort(VertexBuffer, FaceBuffer, FaceColorBuffer, FrontFaceBuffer);

	//Pass to canvas for vertex drawing and rasterization
	Canvas.DrawVertex(ProjectionBuffer, FaceBuffer, FrontFaceBuffer, ColorBuffer, FaceColorBuffer, SightBuffer);
	Canvas.Rasterize(ProjectionBuffer, FaceBuffer, FrontFaceBuffer, ColorBuffer, FaceColorBuffer, SightBuffer);
}

void Renderer::DrawTrace(const std::vector<Polygon*>& PolygonStream, const std::vector<Light*>& LightStream)
{
	if(PolygonStream.size() == 0 || LightStream.size() == 0)
		return;

	//Color buffer for each vertex of each face
	std::vector<Vec3f> PositionBuffer;
	std::vector<Vec3f> DiffuseBuffer;			//Per polygon diffuse color
	std::vector<Vec3f> SpecularBuffer;			//Per polygon specular color

	for(unsigned int p = 0; p < PolygonStream.size(); p++)
	{
		CVM(PolygonStream[p]->GetCenter(), PositionBuffer);
		DiffuseBuffer.push_back(PolygonStream[p]->GetDiffuseColor());
		SpecularBuffer.push_back(PolygonStream[p]->GetSpecularColor());
	}

	Vec2d Size = Canvas.GetDefaultSize() - 1;
	float Range = tan(ViewAngle) * Zmin / 2;
	for(int x = 0; x < Size.x; x++)
	{
		for(int y = 0; y < Size.y; y++)
		{
			//Pixel position in world coordinate
			Vec2f Screen = Vec2f(x, y) / Size;
			Vec3f Point = Vec3f(Screen.x / Range, Screen.y / Range, Zmin);
			Vec3f Direction = (Point - FromVector).normalize();
			Canvas.SetPixelColor(Vec2d(x, y), Trace(FromVector, Direction, PositionBuffer, DiffuseBuffer, SpecularBuffer, LightStream));			
		}
	}
}

void Renderer::Draw(const std::vector<Polygon*>& PolygonStream, const std::vector<Light*>& LightStream, bool RayTrace)
{
	if(RayTrace)
		DrawTrace(PolygonStream, LightStream);
	else
		DrawPhong(PolygonStream, LightStream);

	Render();
}

void Renderer::Render()
{
	Canvas.Render();
}
