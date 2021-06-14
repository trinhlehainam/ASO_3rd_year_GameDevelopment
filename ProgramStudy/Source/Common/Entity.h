#pragma once
#include <memory>

#include "../Math/vec2.h"
#include "../Input/Input.h"

class Entity
{
public:
	Entity(INPUT_DEVICE_ID deviceID);
	explicit Entity(INPUT_DEVICE_ID deviceID, const vec2f& pos, const vec2f& speed);
	~Entity();

	void Update(float deltaTime_s);
	void Render();
private:
	vec2f m_pos;
	vec2f m_speed;

	std::unique_ptr<Input> m_input;
};

