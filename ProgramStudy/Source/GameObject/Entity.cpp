#include "Entity.h"

#include <DxLib.h>

#include "../Utilities/ImageMng.h"
#include "../Input/KeyboardInput.h"
#include "../Input/JoypadXInput.h"

Entity::Entity(INPUT_DEVICE_ID deviceID)
{
	auto& imageMng =  ImageMng::Instance();
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

	for (const auto& component : m_components)
		component->Update(deltaTime_s);
}

void Entity::Render()
{
	for (const auto& component : m_components)
		component->Render();
}
