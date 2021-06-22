#include "Player.h"

#include "Entity.h"
#include "../Input/KeyboardInput.h"
#include "../Input/JoypadXInput.h"
#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init(INPUT_DEVICE_ID deviceId)
{
	m_entity = std::make_shared<Entity>();
	switch (deviceId)
	{
	case INPUT_DEVICE_ID::KEYBOARD:
		m_input = std::make_unique<KeyboardInput>();
		break;
	case INPUT_DEVICE_ID::JOYPAD:
		m_input = std::make_unique<JoypadXInput>();
		break;
	}

	m_entity->AddComponent<TransformComponent>(m_entity, vec2f{ 100.0f,100.0f }, vec2f{ 100.0f ,100.0f }, 1.0f);
	m_entity->AddComponent<SpriteComponent>(m_entity);
	auto sprite = m_entity->GetComponent<SpriteComponent>();
	sprite->Play("knight", "Jump");
}

void Player::Update(float deltaTime_s)
{
	m_input->Update();
	auto speed = vec2f{ 0.0f,0.0f };

	const auto& transform = m_entity->GetComponent<TransformComponent>();
	const auto& sprite = m_entity->GetComponent<SpriteComponent>();

	if (m_input->IsPressed(INPUT_ID::UP))
		speed.y = -100.0f;
	else if (m_input->IsPressed(INPUT_ID::DOWN))
		speed.y = 100.0f;
	else if (m_input->IsPressed(INPUT_ID::LEFT))
		speed.x = -100.0f;
	else if (m_input->IsPressed(INPUT_ID::RIGHT))
		speed.x = 100.0f;
	transform->Pos += speed * deltaTime_s;

	if (speed.x != 0.0f || speed.y != 0.0f)
		sprite->Play("knight", "Run");
	else
		sprite->Play("knight", "Idle");

	m_entity->Update(deltaTime_s);
}

void Player::Render()
{
	m_entity->Render();
}
