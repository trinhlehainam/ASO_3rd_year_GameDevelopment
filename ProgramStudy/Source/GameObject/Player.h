#pragma once
#include <memory>
#include "../Input/Input.h"
#include "../Utilities/StaticRingBuffer.h"

class Entity;
class EntityMng;

class Player
{
public:
	Player(const std::shared_ptr<EntityMng>& entityMng);
	~Player();
public:
	void Init(INPUT_DEVICE_ID deviceId);
	void Update(float deltaTime_s);
	void Render();

	std::shared_ptr<Entity> GetEntity() const;
private:
	std::shared_ptr<Entity> m_entity;
	std::unique_ptr<Input> m_input;
	StaticRingBuffer<int, 5> m_testBuffer;
};

