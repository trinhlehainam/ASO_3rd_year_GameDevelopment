#include "MathHelper.h"

#include <cmath>

#include "../Math/line2.h"
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

	bool isOverlap(const line2& a, const line2& b)
	{
		if (!isParallelVec(a.dir, b.dir))
			return true;
		return line2::isEquivalent(a, b);
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

		}

		return false;
	}
}
