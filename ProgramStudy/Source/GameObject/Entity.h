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
	explicit Entity(INPUT_DEVICE_ID deviceID);
	~Entity();

	void Update(float deltaTime_s);
	void Render();

	template<typename T, typename...Arg>
	void AddComponent(Arg&&...args);

	template<typename T>
	bool HasComponent();

	template<typename T>
	std::shared_ptr<T> GetComponent();

private:
	std::unique_ptr<Input> m_input;
	std::vector<std::shared_ptr<IComponent>> m_components;
	std::unordered_map<const type_info*, std::shared_ptr<IComponent>> m_componentMap;
};

