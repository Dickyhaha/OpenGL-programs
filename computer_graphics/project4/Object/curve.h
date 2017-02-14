#ifndef _CURVE_H_
#define _CURVE_H_

#include "../Core/world_define.h"

class Curve
{
	private:
		std::vector<Vec2f> ControlPoint;
		int Segment;

		Vec2f Center;

		Vec3f Color;

		CURVE CurveType;

		float SpinRate;

		std::vector<float> Knot;
		unsigned int Degree;

	public:
		Curve();
		~Curve();

		float GetSpinRate();
		void SetSpinRate(float NewSpinRate);

		Vec3f GetColor();
		void SetColor(const Vec3f& NewColor);

		CURVE GetCurveType();
		void SetCurveType(CURVE NewCurveType);

		unsigned int GetDegree();
		void SetDegree(unsigned int NewDegree);

		const std::vector<float>& GetKnot();
		void SetKnot(const std::vector<float>& NewKnot);

		const std::vector<Vec2f>& GetControlPoint();
		void SetControlPoint(const std::vector<Vec2f>& NewControlPoint);

		void PopPoint();
		void PushPoint(const Vec2f& NewPoint);

		void RemovePoint(unsigned int Index);
		void AddPoint(unsigned int Index, const Vec2f& NewPoint);

		Vec2f GetPoint(unsigned int Index);
		void SetPoint(unsigned int Index, const Vec2f& NewPoint);

		int GetSegment();
		void SetSegment(int NewSegment);

		void ResetKnot();
		void Calibrate();

		void Translate(const Vec2f& Offset);
		void Rotate(float Angle);
		void Scale(const Vec2f& Factor);

		void Update();
};

#endif
