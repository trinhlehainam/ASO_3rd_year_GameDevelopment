#include "MathHelper.h"

#include <cmath>
#include <algorithm>

#include "../Math/line2.h"
#include "../Math/segment2.h"
#include "../Geometry/AABB.h"
#include "../Geometry/Circle.h"

namespace MathHelper
{
	const float kBiasF = 0.001f;
	const double kBiasD = 0.0001;

	const float kMsToSecond = 1 / 1000.0f;
}

namespace MathHelper
{
	float randf(float num)
	{
		return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / num));
	}

	float randf(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max - min));
	}

	template<typename T>
	bool isEqual(T a, T b)
	{
		return a == b;
	}

#pragma region Specialization
	template <>
	bool isEqual(float a, float b)
	{
		return std::fabsf(a - b) <= kBiasF;
	}

	template <>
	bool isEqual(double a, double b)
	{
		return std::abs(a - b) <= kBiasD;
	}

#pragma endregion


#pragma region Instantiate
	template bool isEqual(int, int);
	template bool isEqual(unsigned int, unsigned int);
	template bool isEqual(short, short);
	
#pragma endregion
};

namespace MathHelper
{
	bool isEquivalent(const line2& a, const line2& b)
	{
		if (!isParallelVec(a.dir, b.dir))
			return false;

		return isParallelVec(a.base - b.base, a.dir);
	}

	bool isOnSameSide(const line2& l, const segment2& s)
	{
		auto orthoVec = orthogonalVec(l.dir);
		return dot(orthoVec, s.a) * dot(orthoVec, s.b) > 0.0f;
	}

	range projectSegmentRange(const segment2& s, const vec2f& onto)
	{
		auto ontoUnit = onto / std::sqrt(dot(onto,onto));

		range r;
		r.min = dot(s.a, onto);
		r.max = dot(s.a, onto);
		std::sort(&r.min, &r.max);

		return r;
	}

	bool isOverlap(const Circle& a, const Circle& b)
	{
		auto d = a.Pos - b.Pos;
		auto r = a.Radius + b.Radius;
		return d * d <= r * r;
	}

	bool isOverlap(const Circle& cir, const AABB& rect)
	{
		return false;
	}

	bool isOverlap(const AABB& rect, const Circle& cir)
	{
		return isOverlap(cir, rect);
	}

	bool isOverlap(const AABB& a, const AABB& b)
	{
		auto aLeft = a.Pos.x;
		auto aRight = a.Pos.x + a.Size.x;
		auto aTop = a.Pos.y;
		auto aBottom = a.Pos.y + a.Size.y;

		auto bLeft = b.Pos.x;
		auto bRight = b.Pos.x + b.Size.x;
		auto bTop = b.Pos.y;
		auto bBottom = b.Pos.y + b.Size.y;

		return isOverlap(aLeft, aRight, bLeft, bRight) && isOverlap(aTop, aBottom, bTop, bBottom);
	}

	bool isOverlap(const line2& a, const line2& b)
	{
		if (!isParallelVec(a.dir, b.dir))
			return true;
		return isEquivalent(a, b);
	}

	bool isOverlap(const segment2& a, const segment2& b)
	{
		line2 axisA{ a.a,a.b - a.a };
		if(isOnSameSide(axisA, b))
			return false;
		
		line2 axisB{ b.a,b.b - b.a };
		if (isOnSameSide(axisB, a))
			return false;

		if (!isParallelVec(axisA.dir, axisB.dir))
			return true;

		return isOverlap(projectSegmentRange(a, axisA.dir), projectSegmentRange(b, axisA.dir));
	}

	bool isOverlap(float minA, float maxA, float minB, float maxB)
	{
		return maxA >= minB && maxA <= maxB;
	}

	bool isOverlap(const range& a, const range& b)
	{
		return isOverlap(a.min, a.max, b.min, b.max);
	}

	bool isOverlap(const IShape* a, const IShape* b)
	{
		if (a->Type == IShape::TYPE::CIRCLE)
		{
			auto circleA = dynamic_cast<const Circle*>(a);

			if (b->Type == IShape::TYPE::CIRCLE)
			{
				auto circleB = dynamic_cast<const Circle*>(b);

				if (isOverlap(*circleA, *circleB))
					return true;
			}
		}
		else if (a->Type == IShape::TYPE::AABB)
		{
			auto rectA = dynamic_cast<const AABB*>(a);

			if (b->Type == IShape::TYPE::AABB)
			{
				auto rectB = dynamic_cast<const AABB*>(b);
				
				if (isOverlap(*rectA, *rectB))
					return true;
			}
		}
		else if (a->Type == IShape::TYPE::CHRISTMAS_TREE)
		{

		}

		return false;
	}
}
