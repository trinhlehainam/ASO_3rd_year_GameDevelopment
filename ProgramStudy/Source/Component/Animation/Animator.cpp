#include "Animator.h"

#include <cassert>
#include <vector>
#include <unordered_map>

#include <rapidxml.hpp>
#include <DxLib.h>

#include "../../Utilities/StringHelper.h"
#include "../../Math/MathHelper.h"
#include "AnimatorState.h"
#include "../../Systems/AnimationMng.h"
#include "../../GameObject/Entity.h"
#include "../TransformComponent.h"

namespace
{
	enum class ANIMATOR_PARAMETER_TYPE
	{
		FLOAT			= 0,
		BOOL			= 1,
		INTEGER			= 2
	};

	struct AnimatorParameter
	{
		ANIMATOR_PARAMETER_TYPE type;
		float value;
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
	void AddParameter(const std::string& name, ANIMATOR_PARAMETER_TYPE paramType);
	void AddParameter(const std::string& name, ANIMATOR_PARAMETER_TYPE paramType, float value);
	bool HasParameter(const std::string& name);

	void Play(const std::string& state);

	bool CheckCondition(const AnimatorCondition& condition);
private:
	void Update(float deltaTime_s);
	void Render();

	void UpdateInfinite(float deltaTime_s);
	void UpdateLoop(float deltaTime_s);
	void UpdateSleep(float deltaTime_s);

	using UpdateFunc_t = void (Impl::*)(float);
	UpdateFunc_t updateFunc;
private:
	friend Animator;

	std::weak_ptr<TransformComponent> transform;

	std::unordered_map<std::string, AnimatorState> stateMap;

	std::unordered_map<std::string, AnimatorParameter> paramMap;

	std::string currentState;
	int currentDurationID;
	int timer_ms;
	int loopCount;
};

Animator::Impl::Impl(const std::shared_ptr<Entity>& owner) :
	transform(owner->GetComponent<TransformComponent>()), currentDurationID(0), timer_ms(0), loopCount(0), updateFunc(&Impl::UpdateSleep) {}
Animator::Impl::~Impl() {}

void Animator::Impl::AddParameter(const std::string& name, ANIMATOR_PARAMETER_TYPE paramType)
{
	assert(!HasParameter(name));
	paramMap.emplace(name, AnimatorParameter{ paramType, 0.0f });
}

void Animator::Impl::AddParameter(const std::string& name, ANIMATOR_PARAMETER_TYPE paramType, float value)
{
	assert(!HasParameter(name));
	paramMap.emplace(name, AnimatorParameter{ paramType, value });
}

bool Animator::Impl::HasParameter(const std::string& name)
{
	return paramMap.count(name);
}

void Animator::Impl::Play(const std::string& state)
{
	if (state == currentState) return;
	currentState = state;
	const auto& animatorState = stateMap[currentState];
	auto& AnimMng = AnimationMng::Instance();

	// Check function's arguments before assign to member variables
	assert(AnimMng.HasAnimation(animatorState.animationList, animatorState.animationState));
	//

	auto& animation = AnimMng.GetAnimation(animatorState.animationList, animatorState.animationState);
	currentDurationID = animation.celBaseId;
	timer_ms = AnimMng.GetDuration_ms(currentDurationID);

	switch (animation.loop)
	{
	case -1:
		updateFunc = &Impl::UpdateInfinite;
		break;
	default:
		updateFunc = &Impl::UpdateLoop;
		loopCount = animation.loop;
		break;
	}

}

bool Animator::Impl::CheckCondition(const AnimatorCondition& condition)
{
	switch (condition.compareMode)
	{
	case CONDITION_MODE::IS_TRUE:
		return static_cast<bool>(paramMap[condition.paramName].value) == true;
	case CONDITION_MODE::IS_FALSE:
		return static_cast<bool>(paramMap[condition.paramName].value) == false;
	case CONDITION_MODE::GREATER:
		return paramMap[condition.paramName].value > condition.threshold;
	case CONDITION_MODE::LESS:
		return paramMap[condition.paramName].value < condition.threshold;
	case CONDITION_MODE::EQUAL:
		return paramMap[condition.paramName].value == condition.threshold;
	case CONDITION_MODE::NOT_EQUAL:
		return paramMap[condition.paramName].value != condition.threshold;
	default:
		return false;
	}

	return false;
}

void Animator::Impl::Update(float deltaTime_s)
{
	// Check conditions
	const auto& state = stateMap[currentState];
	for (const auto& transition : state.transitions)
	{
		for (const auto& condition : transition.conditions)
		{
			if (CheckCondition(condition))
				Play(transition.destinationState);
		}
	}
	//

	(this->*updateFunc)(deltaTime_s);
}

void Animator::Impl::Render()
{
	auto& animMng = AnimationMng::Instance();
	const auto& state = stateMap[currentState];
	const auto& trans = transform.lock();
	const auto& animation = animMng.GetAnimation(state.animationList, state.animationState);
	auto sourceX = (currentDurationID % animation.texColumns) * animation.celWidth;
	auto sourceY = (currentDurationID / animation.texColumns) * animation.celHeight;

	// TODO: Implement pivot(rotation center) variable
	DxLib::DrawRectRotaGraphFast3F(trans->Pos.x, trans->Pos.y,
		sourceX, sourceY, animation.celWidth, animation.celHeight,
		32.0f, 32.0f,
		trans->Scale.x, trans->Scale.y, trans->Rotation, animation.texId, 1);
}

void Animator::Impl::UpdateInfinite(float deltaTime_s)
{
	if (timer_ms <= 0)
	{
		const auto& animatorState = stateMap[currentState];
		auto& animMng = AnimationMng::Instance();
		const auto& animation = animMng.GetAnimation(animatorState.animationList, animatorState.animationState);
		currentDurationID = (currentDurationID - animation.celBaseId + 1) % animation.celCount + animation.celBaseId;
		timer_ms = animMng.GetDuration_ms(currentDurationID);
	}

	timer_ms -= static_cast<int>(deltaTime_s / MathHelper::kMsToSecond);
}

void Animator::Impl::UpdateLoop(float deltaTime_s)
{
	const auto& animatorState = stateMap[currentState];
	auto& animMng = AnimationMng::Instance();
	const auto& animation = animMng.GetAnimation(animatorState.animationList, animatorState.animationState);

	if (timer_ms <= 0)
	{
		++currentDurationID;
		timer_ms = animMng.GetDuration_ms(currentDurationID);
	}

	if (currentDurationID >= (animation.celBaseId + animation.celCount))
	{
		--loopCount;
		currentDurationID = animation.celBaseId;
		timer_ms = animMng.GetDuration_ms(currentDurationID);
	}

	if (loopCount < 0)
	{
		// Last durationId of current animation
		currentDurationID = animation.celBaseId + animation.celCount - 1;
		updateFunc = &Impl::UpdateSleep;
		return;
	}

	timer_ms -= static_cast<int>(deltaTime_s / MathHelper::kMsToSecond);
}

void Animator::Impl::UpdateSleep(float deltaTime_s)
{
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

	std::string animtorName;
	std::string entryState;
	for (auto pAttr = pAnimController->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "name") == 0)
			animtorName = std::move(pAttr->value());
		else if (strcmp(pAttr->name(), "entryState") == 0)
			entryState = std::move(pAttr->value());
	}
	m_impl->currentState = std::move(entryState);

	// Load Parameters
	for (auto pParameter = pAnimController->first_node("parameter"); pParameter; pParameter = pParameter->next_sibling())
	{
		if (strcmp(pParameter->name(), "parameter") != 0)
			break;

		std::string paraName;
		ANIMATOR_PARAMETER_TYPE type{};
		float value = 0.0f;
		for (auto pAttr = pParameter->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (strcmp(pAttr->name(), "name") == 0)
				paraName = std::move(pAttr->value());
			else if (strcmp(pAttr->name(), "type") == 0)
				type = static_cast<ANIMATOR_PARAMETER_TYPE>(std::atoi(pAttr->value()));
			else if (strcmp(pAttr->name(), "value") == 0)
				value = std::strtof(std::move(pAttr->value()), 0);
		}
		m_impl->AddParameter(paraName, type, value);
	}
	//

	for (auto pState = pAnimController->first_node("animatorState"); pState; pState = pState->next_sibling())
	{
		// Load AnimationState
		std::string stateName;
		std::string animList;
		std::string animState;
		for (auto pAttr = pState->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (strcmp(pAttr->name(), "name") == 0)
				stateName = std::move(pAttr->value());
			else if (strcmp(pAttr->name(), "animationList") == 0)
				animList = std::move(pAttr->value());
			else if (strcmp(pAttr->name(), "animationState") == 0)
				animState = std::move(pAttr->value());
		}
		AnimatorState state{};
		state.animationList = std::move(animList);
		state.animationState = std::move(animState);
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
	assert(m_impl->HasParameter(name));
	assert(m_impl->paramMap[name].type == ANIMATOR_PARAMETER_TYPE::FLOAT);
	m_impl->paramMap[name].value = value;
}

void Animator::SetBool(const std::string& name, bool flag)
{
	assert(m_impl->HasParameter(name));
	assert(m_impl->paramMap[name].type == ANIMATOR_PARAMETER_TYPE::BOOL);
	m_impl->paramMap[name].value = static_cast<float>(flag);
}

void Animator::SetInteger(const std::string& name, int value)
{
	assert(m_impl->HasParameter(name));
	assert(m_impl->paramMap[name].type == ANIMATOR_PARAMETER_TYPE::INTEGER);
	m_impl->paramMap[name].value = static_cast<float>(value);
}

float Animator::GetFloat(const std::string& name)
{
	assert(m_impl->HasParameter(name));
	assert(m_impl->paramMap[name].type == ANIMATOR_PARAMETER_TYPE::FLOAT);
	return m_impl->paramMap[name].value;
}

bool Animator::GetBool(const std::string& name)
{
	assert(m_impl->HasParameter(name));
	assert(m_impl->paramMap[name].type == ANIMATOR_PARAMETER_TYPE::BOOL);
	return static_cast<bool>(m_impl->paramMap[name].value);
}

int Animator::GetInteger(const std::string& name)
{
	assert(m_impl->HasParameter(name));
	assert(m_impl->paramMap[name].type == ANIMATOR_PARAMETER_TYPE::INTEGER);
	return static_cast<int>(m_impl->paramMap[name].value);
}

void Animator::Play(const std::string& animatorState)
{
	m_impl->Play(animatorState);
}

void Animator::Init()
{
}

void Animator::Update(float deltaTime_s)
{
	m_impl->Update(deltaTime_s);
}

void Animator::Render()
{
	m_impl->Render();
}