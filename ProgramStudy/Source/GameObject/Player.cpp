#include "Player.h"

#include "../_debug/_DebugDispOut.h"
#include "../_debug/_DebugConOut.h"

#include "Entity.h"

#include "../Systems/ImageMng.h"
#include "../Systems/Physics.h"
#include "../Systems/EntityMng.h"

#include "../Input/KeyboardInput.h"
#include "../Input/JoypadXInput.h"
#include "../Input/InputCommand.h"

#include "../Component/TransformComponent.h"
#include "../Component/Collider/BoxCollider.h"
#include "../Component/Collider/CircleCollider.h"
#include "../Component/Animation/Animator.h"

namespace
{
	int color = 0;
	vec2f dir;
}

Player::Player(const std::shared_ptr<EntityMng>& entityMng):m_entity(std::make_shared<Entity>(entityMng))
{
	entityMng->AddEntity(m_entity);
}

Player::~Player()
{
}

void Player::Init(INPUT_DEVICE_ID deviceId)
{
	ImageMng::AddImage("knight", "Assets/Textures/knight 1 axe.png");

	switch (deviceId)
	{
	case INPUT_DEVICE_ID::KEYBOARD:
		m_input = std::make_shared<KeyboardInput>();
		break;
	case INPUT_DEVICE_ID::JOYPAD:
		m_input = std::make_shared<JoypadXInput>();
		break;
	}

	m_inputCommand = std::make_shared<InputCommand>(m_input);
	m_inputCommand->LoadPatternFromXML("Assets/InputPatterns/inputpattern.xml");

	m_entity->SetTag("kinght");
	m_entity->AddComponent<TransformComponent>(m_entity);
	auto transform = m_entity->GetComponent<TransformComponent>();
	transform->Pos = vec2f{ 100.0f,100.0f };
	transform->Scale = vec2f{ 1.5f,1.5f };
	// m_entity->AddComponent<CircleCollider>(m_entity);
	// auto collider = m_entity->GetComponent<CircleCollider>();
	// collider->SetCenterPos(vec2f{ 100.0f, 100.0f });
	// collider->SetRadius(32.0f);

	m_entity->AddComponent<Animator>(m_entity);
	auto animator = m_entity->GetComponent<Animator>();
	animator->AddAnimatorController("playerAnimator");

}

void Player::Update(float deltaTime_s)
{
	m_input->Update();
	m_inputCommand->Update();
	auto speed = vec2f{ 0.0f,0.0f };

	const auto& transform = m_entity->GetComponent<TransformComponent>();
	auto animator = m_entity->GetComponent<Animator>();

	if (m_input->IsPressed(INPUT_ID::UP))
		speed.y = -100.0f;
	else if (m_input->IsPressed(INPUT_ID::DOWN))
		speed.y = 100.0f;
	else if (m_input->IsPressed(INPUT_ID::LEFT))
		speed.x = -100.0f;
	else if (m_input->IsPressed(INPUT_ID::RIGHT))
		speed.x = 100.0f;
	transform->Pos += speed * deltaTime_s;

	animator->SetFloat("speed", dot(speed, speed));

	animator->SetBool("isAttack", m_inputCommand->IsMatch("combo-1", 1.0f));

	animator->SetBool("isImpact", m_inputCommand->IsMatch("combo-2", 0.5f));

	if (speed != 0.0f)
	{
		dir = unitVec(speed);
		color = Physics::RayCast(transform->Pos, dir, 50.0f) ? 0xff0000 : 0x00ff00;
	}

}

void Player::Render()
{
#ifdef _DEBUG || DEBUG
	const auto& transform = m_entity->GetComponent<TransformComponent>();
	vec2f origin{ transform->Pos };
	vec2f end = origin + dir * 50.0f;
	DxLib::DrawLineAA(origin.x, origin.y, end.x, end.y, color, 2.0f);
#endif
}

std::shared_ptr<Entity> Player::GetEntity() const
{
	return m_entity;
}
