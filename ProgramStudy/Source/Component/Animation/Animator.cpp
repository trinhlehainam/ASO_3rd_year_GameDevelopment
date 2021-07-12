#include "Animator.h"

#include <cassert>
#include <vector>
#include <unordered_map>

#include <rapidxml.hpp>

#include "../../Utilities/StringHelper.h"
#include "AnimatorState.h"
#include "../../GameObject/Entity.h"
#include "../TransformComponent.h"

namespace
{
	enum class PARAMETER_TYPE
	{
		FLOAT,
		BOOL,
		INTEGER
	};
}

namespace
{
	constexpr float kDefaultFloat = 0.0f;
	constexpr bool kDefaultBool = false;
	constexpr int kDefaultInt = 0;
}

#pragma region Pimpl
class Animator::Impl
{
public:
	Impl(const std::shared_ptr<Entity>& owner);
	~Impl();

private:
	void AddParameter(const std::string& name, PARAMETER_TYPE paramType);
	bool HasParameter(const std::string& name);
private:
	friend Animator;

	std::weak_ptr<TransformComponent> transform;

	std::unordered_map<std::string, AnimatorState> stateMap;

	// Parameters
	std::unordered_map<std::string, float> floatParams;
	std::unordered_map<std::string, bool> boolParams;
	std::unordered_map<std::string, int> intParams;

	std::string currentAnimState;
	int currentDurationID;
	int timer_ms;
	int loopCount;
};

Animator::Impl::Impl(const std::shared_ptr<Entity>& owner) :
	transform(owner->GetComponent<TransformComponent>()), currentDurationID(0), timer_ms(0), loopCount(0) {}
Animator::Impl::~Impl() {}

void Animator::Impl::AddParameter(const std::string& name, PARAMETER_TYPE paramType)
{
	assert(HasParameter(name));
	switch (paramType)
	{
	case PARAMETER_TYPE::FLOAT:
		floatParams.emplace(name, kDefaultFloat);
		break;
	case PARAMETER_TYPE::BOOL:
		boolParams.emplace(name, kDefaultBool);
		break;
	case PARAMETER_TYPE::INTEGER:
		intParams.emplace(name, kDefaultInt);
		break;
	default:
		break;
	}
}

bool Animator::Impl::HasParameter(const std::string& name)
{
	return floatParams.count(name) || intParams.count(name) || boolParams.count(name);
}

#pragma endregion


Animator::Animator(const std::shared_ptr<Entity>& owner):
	IComponent(owner),m_impl(std::make_unique<Impl>(owner))
{
}

Animator::~Animator()
{
}

void Animator::AddAnimatorController(const std::string& path)
{
	rapidxml::xml_document<> doc;
	auto content = StringHelper::LoadFileToStringBuffer(path);
	doc.parse<0>(&content[0]);

	auto pAnimController = doc.first_node();
	for (auto pState = pAnimController->first_node(); pState; pState = pState->next_sibling())
	{
		// Load AnimationState
		std::string stateName;
		std::string motion;
		for (auto pAttr = pState->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (strcmp(pAttr->name(), "name") == 0)
				stateName = std::move(pAttr->value());
			else if (strcmp(pAttr->name(), "motion") == 0)
				motion = std::move(pAttr->value());
		}
		AnimatorState state{};
		state.motionName = std::move(motion);
		state.tag = stateName;
		//

		// Load Transitions
		for (auto pTransition = pState->first_node(); pTransition; pTransition = pTransition->next_sibling())
		{
			std::string transName;
			std::string currentState;
			std::string destState;
			for (auto pAttr = pTransition->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				if (strcmp(pAttr->name(), "name") == 0)
					transName = std::move(pAttr->value());
				else if (strcmp(pAttr->name(), "currentState") == 0)
					currentState = std::move(pAttr->value());
				else if (strcmp(pAttr->name(), "destinationState") == 0)
					destState = std::move(pAttr->value());
			}

			AnimatorTransition transition{};
			transition.tag = transName;
			transition.currentState = std::move(currentState);
			transition.destinationState = std::move(destState);

			// Load Conditions
			for (auto pCondition = pTransition->first_node(); pCondition; pCondition = pCondition->next_sibling())
			{
				CONDITION_MODE mode{};
				std::string paraName;
				float threshold = 0.0f;
				for (auto pAttr = pCondition->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
				{
					if (strcmp(pAttr->name(), "mode") == 0)
						mode = static_cast<CONDITION_MODE>(std::atoi(pAttr->value()));
					else if (strcmp(pAttr->name(), "paraName") == 0)
						paraName = std::move(pAttr->value());
					else if (strcmp(pAttr->name(), "threshold") == 0)
						threshold = std::strtof(std::move(pAttr->value()), 0);
				}
				transition.AddCondition(mode, paraName, threshold);
			}
			state.transitions.push_back(transition);
		}

		m_impl->stateMap.emplace(stateName, state);
	}

	doc.clear();
}

void Animator::SetFloat(const std::string& name, float value)
{
	assert(m_impl->floatParams.count(name));
	m_impl->floatParams[name] = value;
}

void Animator::SetBool(const std::string& name, bool flag)
{
	assert(m_impl->boolParams.count(name));
	m_impl->boolParams[name] = flag;
}

void Animator::SetInteger(const std::string& name, int value)
{
	assert(m_impl->intParams.count(name));
	m_impl->intParams[name] = value;
}

float Animator::GetFloat(const std::string& name)
{
	assert(m_impl->floatParams.count(name));
	return m_impl->floatParams[name];
}

bool Animator::GetBool(const std::string& name)
{
	assert(m_impl->boolParams.count(name));
	return m_impl->boolParams[name];
}

int Animator::GetInteger(const std::string& name)
{
	assert(m_impl->intParams.count(name));
	return m_impl->intParams[name];
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