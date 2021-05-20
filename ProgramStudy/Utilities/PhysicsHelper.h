#pragma once

class AABB;
class Circle;
class IShape;
struct line2;

namespace PhysicsHelper
{
	bool CheckCollision(const IShape* a, const IShape* b);

	bool CheckCollision(const Circle& a, const Circle& b);

	bool CheckCollision(const Circle& cir, const AABB& rect);

	bool CheckCollision(const AABB& rect, const Circle& cir);

	bool CheckCollision(const line2& a, const line2& b);
};

