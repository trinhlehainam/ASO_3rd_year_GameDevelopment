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
	static void Create();
	static void Destroy();
	static Physics& Instance();
public:
	static bool RayCast(const vec2f& origin, const vec2f& dir, float maxDistance);
	static void AddCollider(const std::shared_ptr<ICollider>& collider);

private:
	Physics();
	~Physics();
private:
	 std::vector<std::weak_ptr<ICollider>> m_colliders;
	 static Physics* m_instance;
};

