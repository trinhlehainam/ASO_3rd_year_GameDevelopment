#include "Animator.h"

#include <cassert>

#include "../GameObject/Entity.h"
#include "TransformComponent.h"

namespace
{
	constexpr float kDefaultFloat = 0.0f;
	constexpr bool kDefaultBool = false;
	constexpr int kDefaultInt = 0;
}

Animator::Animator(const std::shared_ptr<Entity>& owner):
	IComponent(owner),m_tranform(owner->GetComponent<TransformComponent>()),
	m_currentDurationId(0), m_timer_ms(0), m_loopCount(0)
{
}

Animator::~Animator()
{
}

void Animator::AddParameter(const std::string& name, PARAMETER_TYPE paramType)
{
	assert(HasParameter(name));
	switch (paramType)
	{
	case PARAMETER_TYPE::FLOAT:
		m_floatParams.emplace(name, kDefaultFloat);
		break;
	case PARAMETER_TYPE::BOOL:
		m_boolParams.emplace(name, kDefaultBool);
		break;
	case PARAMETER_TYPE::INTEGER:
		m_intParams.emplace(name, kDefaultInt);
		break;
	default:
		break;
	}
}

void Animator::SetFloat(const std::string& name, float value)
{
	assert(m_floatParams.count(name));
	m_floatParams[name] = value;
}

void Animator::SetBool(const std::string& name, bool flag)
{
	assert(m_boolParams.count(name));
	m_boolParams[name] = flag;
}

void Animator::SetInteger(const std::string& name, int value)
{
	assert(m_intParams.count(name));
	m_intParams[name] = value;
}

float Animator::GetFloat(const std::string& name)
{
	assert(m_floatParams.count(name));
	return m_floatParams[name];
}

bool Animator::GetBool(const std::string& name)
{
	assert(m_boolParams.count(name));
	return m_boolParams[name];
}

int Animator::GetInteger(const std::string& name)
{
	assert(m_intParams.count(name));
	return m_intParams[name];
}

void Animator::Play(const std::string& state)
{

}

void Animator::Init()
{
}

void Animator::Update(float deltaTime_s)
{
}

void Animator::Render()
{
}

bool Animator::HasParameter(const std::string& name)
{
	return m_floatParams.count(name) || m_intParams.count(name) || m_boolParams.count(name);
}
