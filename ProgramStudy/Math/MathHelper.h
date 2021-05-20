#pragma once

namespace MathHelper
{
	// Floating point numbers are rarely exact the same.
	// -> Two floating point numbers are equal when they are in range of below bias
	extern const float kBiasF;
	extern const double kBiasD;
	//

	extern const float kMsToSecond;
}

class AABB;
class Circle;
class IShape;
struct line2;
struct segment2;

namespace MathHelper
{
	float randf(float num);
	float randf(float min, float max);
	template<typename T> bool isEqual(T a, T b);

	bool isOverlap(const IShape* a, const IShape* b);
	bool isOverlap(const Circle& a, const Circle& b);
	bool isOverlap(const Circle& cir, const AABB& rect);
	bool isOverlap(const AABB& rect, const Circle& cir);
	bool isOverlap(const line2& a, const line2& b);
};

