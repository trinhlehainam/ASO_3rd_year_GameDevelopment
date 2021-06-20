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

	m_entity->AddComponent<TransformComponent>(m_entity, vec2f{ 100.0f,100.0f }, vec2f{ 100.0f,100.0f }, 1.0f);
	m_entity->AddComponent<SpriteComponent>(m_entity);
	auto sprite = m_entity->GetComponent<SpriteComponent>();
	sprite->LoadAnimationFromXML("Assets/Animations/animation.xml", "knight");
	sprite->Play("knight", "Jump");
}

void Player::Update(float deltaTime_s)
{
	m_input->Update();

	m_entity->Update(deltaTime_s);
}

void Player::Render()
{
	m_entity->Render();
}
