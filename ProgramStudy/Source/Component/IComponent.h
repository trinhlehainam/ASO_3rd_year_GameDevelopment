#pragma once
#include <memory>

class Entity;

class IComponent
{
public:
	explicit IComponent(const std::shared_ptr<Entity>& owner);
	virtual ~IComponent();

	virtual void Init() = 0;
	virtual void Update(float deltaTime_s) = 0;
	virtual void Render() = 0;

	inline void SetOwner(const std::shared_ptr<Entity>& owner) { m_owner = owner; }
	inline bool IsOwnerExist() { return !m_owner.expired(); }

private:
	std::weak_ptr<Entity> m_owner;
};

