#include "AABB.h"

#include <algorithm>
#include "../Math/MathHelper.h"

AABB::AABB() = default;

AABB::AABB(const vec2f& pos, const vec2f& size) : Pos(pos), Size(size) {}
void AABB::operator=(const AABB& other)
{
	Pos = other.Pos;
	Size = other.Size;
}
void AABB::operator=(AABB&& other) noexcept
{
	std::swap(Pos, other.Pos);
	std::swap(Size, other.Size);
}
AABB::~AABB() = default;

