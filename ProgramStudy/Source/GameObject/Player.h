#pragma once
#include <memory>
#include "../Input/Input.h"

class Entity;

class Player
{
public:
	Player();
	~Player();
public:
	void Init(INPUT_DEVICE_ID deviceId);
	void Update(float deltaTime_s);
	void Render();
private:
	std::shared_ptr<Entity> m_entity;
	std::unique_ptr<Input> m_input;
};

