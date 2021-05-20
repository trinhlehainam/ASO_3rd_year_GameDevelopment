#pragma once

class AABB;
class Circle;
class IShape;

namespace PhysicsHelper
{
	bool CheckCollision(const Circle& a, const Circle& b);

	bool CheckCollision(const Circle& cir, const AABB& rect);

	bool CheckCollision(const AABB& rect, const Circle& cir);

	bool CheckCollision(const IShape* a, const IShape* b);
};

