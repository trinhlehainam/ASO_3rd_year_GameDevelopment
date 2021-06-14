#include "Entity.h"

#include <DxLib.h>

#include "../Utilities/ImageMng.h"

Entity::Entity()
{
	auto& imageMng =  ImageMng::Instance();
	imageMng.AddImage("knight", "Assets/Textures/knight 1 axe.png");
}

Entity::Entity(const vec2f& pos, const vec2f& speed):m_pos(pos),m_speed(speed)
{
	auto& imageMng = ImageMng::Instance();
	imageMng.AddImage("knight", "Assets/Textures/knight 1 axe.png");
}

Entity::~Entity()
{
}

void Entity::Update(float deltaTime_s)
{
	m_input.Update();

	if (m_input.IsJustPressed(INPUT_ID::DOWN))
		m_pos.y += m_speed.y* deltaTime_s;
	else if(m_input.IsJustPressed(INPUT_ID::UP))
		m_pos.y -= m_speed.y * deltaTime_s;
	else if(m_input.IsJustPressed(INPUT_ID::LEFT))
		m_pos.x -= m_speed.x * deltaTime_s;
	else if(m_input.IsJustPressed(INPUT_ID::RIGHT))
		m_pos.x += m_speed.x * deltaTime_s;
}

void Entity::Render()
{
	auto& imageMng = ImageMng::Instance();
	DxLib::DrawRectGraphF(m_pos.x, m_pos.y, 0, 0, 64, 64, imageMng.GetID("knight"), 1);
}
