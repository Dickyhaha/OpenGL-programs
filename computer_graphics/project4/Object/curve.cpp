#include "curve.h"

Curve::Curve(): Segment(0), Color(WHITE), CurveType(BEZIER), Degree(1)
{
}

Curve::~Curve()
{
}

float Curve::GetSpinRate()
{
	return SpinRate;
}

void Curve::SetSpinRate(float NewSpinRate)
{
	SpinRate = NewSpinRate;
}

Vec3f Curve::GetColor()
{
	return Color;
}

void Curve::SetColor(const Vec3f& NewColor)
{
	Color = NewColor;
}

CURVE Curve::GetCurveType()
{
	return CurveType;
}

void Curve::SetCurveType(CURVE NewCurveType)
{
	CurveType = NewCurveType;
}

unsigned int Curve::GetDegree()
{
	return Degree;
}

void Curve::SetDegree(unsigned int NewDegree)
{
	Degree = NewDegree;
	ResetKnot();
}

const std::vector<float>& Curve::GetKnot()
{
	return Knot;
}

void Curve::SetKnot(const std::vector<float>& NewKnot)
{
	Knot = NewKnot;
}

const std::vector<Vec2f>& Curve::GetControlPoint()
{
	return ControlPoint;
}

void Curve::SetControlPoint(const std::vector<Vec2f>& NewControlPoint)
{
	ControlPoint = NewControlPoint;
	ResetKnot();
	Calibrate();
}

void Curve::PopPoint()
{
	ControlPoint.pop_back();
	ResetKnot();
	Calibrate();
}

void Curve::PushPoint(const Vec2f& NewPoint)
{
	ControlPoint.push_back(NewPoint);
	ResetKnot();
	Calibrate();
}

void Curve::RemovePoint(unsigned int Index)
{	
	ControlPoint.erase(ControlPoint.begin() + Index);
	ResetKnot();
	Calibrate();
}

void Curve::AddPoint(unsigned int Index, const Vec2f& NewPoint)
{
	ControlPoint.insert(ControlPoint.begin() + Index, NewPoint);
	ResetKnot();
	Calibrate();
}

Vec2f Curve::GetPoint(unsigned int Index)
{
	if(Index < 0 || Index >= ControlPoint.size())
		return Vec2f();
	return ControlPoint[Index];
}

void Curve::SetPoint(unsigned int Index, const Vec2f& NewPoint)
{
	if(Index < 0 || Index >= ControlPoint.size())
		return;
	ControlPoint[Index] = NewPoint;
	Calibrate();
}

int Curve::GetSegment()
{
	return Segment;
}

void Curve::SetSegment(int NewSegment)
{
	Segment = NewSegment;
}

void Curve::Translate(const Vec2f& Offset)
{
	for(unsigned int p = 0; p < ControlPoint.size(); p++)
		ControlPoint[p] += Offset;
	Calibrate();
}

void Curve::Rotate(float Angle)
{
	float sinA = sin(Angle);
	float cosA = cos(Angle);

	Vec2f temp;

	for(unsigned int p = 0; p < ControlPoint.size(); p++)
	{
		temp = ControlPoint[p] - Center;
		ControlPoint[p].x = cosA * temp.x - sinA * temp.y + Center.x;
		ControlPoint[p].y = sinA * temp.x + cosA * temp.y + Center.y;
	}
	Calibrate();
}

void Curve::Scale(const Vec2f& Factor)
{
	for(unsigned int p = 0; p < ControlPoint.size(); p++)
		ControlPoint[p] = (ControlPoint[p] - Center) * Factor + Center;
	Calibrate();
}

void Curve::ResetKnot()
{
	Knot.clear();
	for(unsigned int i = 0; i < Degree + ControlPoint.size(); i++)
		Knot.push_back(i);
}

void Curve::Calibrate()
{
	Center = Vec2f();
	if(ControlPoint.size() != 0)
	{
		for(unsigned int p = 0; p < ControlPoint.size(); p++)
			Center += ControlPoint[p];
		Center = Center / ControlPoint.size();
	}
}

void Curve::Update()
{
	if(SpinRate != 0.0f)
		Rotate(SpinRate);
}
