#pragma once
#include <memory>
#include <vector>

template<typename T>
struct vec2;

using vec2f = vec2<float>;

class ICollider;

class Physics
{
public:
	static bool RayCast(const vec2f& origin, const vec2f& dir, float maxDistance);

private:
	static std::vector<std::weak_ptr<ICollider>> m_colliders;
};

