#pragma once
#include "../Math/vec2.h"

struct AABB
{
	AABB();
	AABB(const vec2f& pos, const vec2f& size);
	AABB(const AABB&) = default;
	AABB(AABB&&) noexcept = default;
	void operator = (const AABB&);
	void operator = (AABB&&) noexcept;
	~AABB();

	vec2f Pos;
	vec2f Size;
};

