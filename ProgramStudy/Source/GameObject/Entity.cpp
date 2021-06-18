#include "Entity.h"

#include <DxLib.h>

#include "../ImageMng.h"
#include "../Input/KeyboardInput.h"
#include "../Input/JoypadXInput.h"

#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"

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

template<typename T, typename...Arg>
void Entity::AddComponent(Arg&&...args)
{
	auto component = std::make_shared<T>(std::forward<Arg>(args)...);
	component->Init();
	m_components.emplace_back(component);
	m_componentMap.emplace(&typeid(T), component);
}

template<typename T>
bool Entity::HasComponent()
{
	return m_componentMap.count(&typeid(T));
}

template<typename T>
std::shared_ptr<T> Entity::GetComponent()
{
	if (!HasComponent<T>()) return nullptr;
	return std::static_pointer_cast<T>(m_componentMap.at(&typeid(T)));
}
			
#define Instantiate(component)\
template bool Entity::HasComponent<component>();\
template std::shared_ptr<component> Entity::GetComponent<>();\

#define ExplicitInstantiate(component, ...)\
template void Entity::AddComponent<component, std::shared_ptr<Entity>&, __VA_ARGS__>(std::shared_ptr<Entity>&, __VA_ARGS__); \


#pragma region Instantiation

template void Entity::AddComponent<TransformComponent, std::shared_ptr<Entity>&, vec2f, vec2f, float>(
	std::shared_ptr<Entity>&, vec2f&&, vec2f&&, float&&);
template void Entity::AddComponent<SpriteComponent, std::shared_ptr<Entity>&>(std::shared_ptr<Entity>&);

Instantiate(TransformComponent);
Instantiate(SpriteComponent);
#pragma endregion

