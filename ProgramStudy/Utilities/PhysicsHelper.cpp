#include "PhysicsHelper.h"

#include "../Math/line2.h"
#include "../Geometry/AABB.h"
#include "../Geometry/Circle.h"

namespace PhysicsHelper
{
	bool CheckCollision(const Circle& a, const Circle& b)
	{
		auto d = a.Pos - b.Pos;
		auto r = a.Radius + b.Radius;
		return d * d <= r * r;
	}

	bool CheckCollision(const Circle& cir, const AABB& rect)
	{
		return false;
	}

	bool CheckCollision(const AABB& rect, const Circle& cir)
	{
		return CheckCollision(cir, rect);
	}

	bool CheckCollision(const line2& a, const line2& b)
	{
		if (!isParallelVec(a.dir, b.dir))
			return true;
		return isEquivalent(a,b);
	}

	bool CheckCollision(const IShape* a, const IShape* b)
	{
		if (a->Type == IShape::TYPE::CIRCLE)
		{
			auto circleA = dynamic_cast<const Circle*>(a);

			if (b->Type == IShape::TYPE::CIRCLE)
			{
				auto circleB = dynamic_cast<const Circle*>(b);

				if (CheckCollision(*circleA, *circleB))
					return true;
			}
		}
		else if (a->Type == IShape::TYPE::AABB)
		{

		}

		return false;
	}
}
