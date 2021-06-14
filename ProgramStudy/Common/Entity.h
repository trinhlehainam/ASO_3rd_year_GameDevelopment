#pragma once
#include "../Math/vec2.h"
#include "../Input/Input.h"

class Entity
{
public:
	Entity();
	explicit Entity(const vec2f& pos, const vec2f& speed);
	~Entity();

	void Update(float deltaTime_s);
	void Render();
private:
	vec2f m_pos;
	vec2f m_speed;

	Input m_input;
};

