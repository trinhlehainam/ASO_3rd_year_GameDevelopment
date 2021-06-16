#include "Entity.h"

#include <DxLib.h>

#include "../Utilities/ImageMng.h"
#include "../Input/KeyboardInput.h"
#include "../Input/JoypadXInput.h"

Entity::Entity(INPUT_DEVICE_ID deviceID)
{
	auto& imageMng =  ImageMng::Instance();
	imageMng.AddImage("knight", "Assets/Textures/knight 1 axe.png");
}

Entity::Entity(INPUT_DEVICE_ID deviceID, const vec2f& pos, const vec2f& speed):m_pos(pos),m_speed(speed)
{
	auto& imageMng = ImageMng::Instance();
	imageMng.AddImage("knight", "Assets/Textures/knight 1 axe.png");
	
	switch (deviceID)
	{
	case INPUT_DEVICE_ID::KEYBOARD:
		m_input = std::make_unique<KeyboardInput>();
		break;
	case INPUT_DEVICE_ID::JOYPAD:
		m_input = std::make_unique<JoypadXInput>();
		break;
	}
}

Entity::~Entity()
{
}

void Entity::Update(float deltaTime_s)
{
	m_input->Update();

	if (m_input->IsPressed(INPUT_ID::DOWN))
		m_pos.y += m_speed.y* deltaTime_s;
	else if(m_input->IsPressed(INPUT_ID::UP))
		m_pos.y -= m_speed.y * deltaTime_s;
	else if(m_input->IsPressed(INPUT_ID::LEFT))
		m_pos.x -= m_speed.x * deltaTime_s;
	else if(m_input->IsPressed(INPUT_ID::RIGHT))
		m_pos.x += m_speed.x * deltaTime_s;
}

void Entity::Render()
{
	auto& imageMng = ImageMng::Instance();
	DxLib::DrawRectGraphF(m_pos.x, m_pos.y, 0, 0, 64, 64, imageMng.GetID("knight"), 1);
}
