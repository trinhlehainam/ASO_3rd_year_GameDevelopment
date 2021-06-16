#include "TransformComponent.h"

TransformComponent::TransformComponent(const std::shared_ptr<Entity>& owner, const vec2f& pos, const vec2f& size, float scale):
	IComponent(owner),
	Pos(pos),
	Size(size),
	Scale(scale)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Init()
{
}

void TransformComponent::Update(float deltaTime_s)
{

}

void TransformComponent::Render()
{
}
