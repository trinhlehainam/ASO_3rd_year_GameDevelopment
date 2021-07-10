#pragma once
#include <memory>
#include "../Input/IInput.h"
#include "../Utilities/static_ring.h"

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
	std::unique_ptr<IInput> m_input;
	static_ring<int, 5> m_testBuffer;
};

