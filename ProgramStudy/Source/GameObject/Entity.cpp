#include "Entity.h"

#include <DxLib.h>

#include "../Systems/ImageMng.h"

#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"

Entity::Entity()
{
	auto& imageMng =  ImageMng::Instance();
	imageMng.AddImage("knight", "Assets/Textures/knight 1 axe.png");

}

Entity::~Entity()
{
}

void Entity::Update(float deltaTime_s)
{
	for (const auto& component : m_components)
		component->Update(deltaTime_s);
}

void Entity::Render()
{
	for (const auto& component : m_components)
		component->Render();
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
			
#define InstantiateFuncTemplate(component)\
template bool Entity::HasComponent<component>();\
template std::shared_ptr<component> Entity::GetComponent<>();\

#pragma region Instantiation
InstantiateFuncTemplate(TransformComponent);
InstantiateFuncTemplate(SpriteComponent);
#pragma endregion

