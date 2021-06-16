#include "IComponent.h"

IComponent::IComponent(const std::shared_ptr<Entity>& owner) :m_owner(owner) {}

IComponent::~IComponent() = default;
