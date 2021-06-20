#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeinfo>

#include "../Component/IComponent.h"
#include "../Math/vec2.h"
#include "../Input/Input.h"

class Entity
{
public:
	explicit Entity();
	~Entity();

	void Update(float deltaTime_s);
	void Render();

	template<typename T, typename...Arg>
	void AddComponent(Arg&&...args)
	{
		auto component = std::make_shared<T>(std::forward<Arg>(args)...);
		component->Init();
		m_components.emplace_back(component);
		m_componentMap.emplace(&typeid(T), component);
	}

	template<typename T>
	bool HasComponent();

	template<typename T>
	std::shared_ptr<T> GetComponent();

private:
	std::vector<std::shared_ptr<IComponent>> m_components;
	std::unordered_map<const type_info*, std::shared_ptr<IComponent>> m_componentMap;
};

