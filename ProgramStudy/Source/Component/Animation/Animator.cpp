#include "Animator.h"

#include <cassert>
#include <vector>
#include <unordered_map>

#include <DxLib.h>

#include "../TransformComponent.h"

#include "../../Systems/AnimatorControllerMng.h"
#include "../../Math/MathHelper.h"
#include "../../Systems/AnimationMng.h"
#include "../../GameObject/Entity.h"


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

	std::string animatorKey;

	std::string currentState;
	int currentDurationID;
	int timer_ms;
	int loopCount;
};

Animator::Impl::Impl(const std::shared_ptr<Entity>& owner) :
	updateFunc(&Impl::UpdateSleep),
	transform(owner->GetComponent<TransformComponent>()), 
	currentDurationID(0), timer_ms(0), loopCount(0) 
	{}
Animator::Impl::~Impl() { }

void Animator::Impl::Play(const std::string& state)
{
	if (!AnimatorControllerMng::Has(animatorKey)) return;
	const auto& animator = AnimatorControllerMng::Get(animatorKey);
	if (state == currentState) return;
	currentState = state;
	const auto& animatorState = animator.stateMap.at(currentState);
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
	if (!AnimatorControllerMng::Has(animatorKey)) return false;
	const auto& animator = AnimatorControllerMng::Get(animatorKey);
	switch (condition.compareMode)
	{
	case CONDITION_MODE::IS_TRUE:
		return static_cast<bool>(animator.paramMap.at(condition.paramName).value) == true;
	case CONDITION_MODE::IS_FALSE:		 
		return static_cast<bool>(animator.paramMap.at(condition.paramName).value) == false;
	case CONDITION_MODE::GREATER:
		return animator.paramMap.at(condition.paramName).value > condition.threshold;
	case CONDITION_MODE::LESS:
		return animator.paramMap.at(condition.paramName).value < condition.threshold;
	case CONDITION_MODE::EQUAL:
		return animator.paramMap.at(condition.paramName).value == condition.threshold;
	case CONDITION_MODE::NOT_EQUAL:
		return animator.paramMap.at(condition.paramName).value != condition.threshold;
	default:
		return false;
	}

	return false;
}

void Animator::Impl::Update(float deltaTime_s)
{
	if (!AnimatorControllerMng::Has(animatorKey)) return;
	const auto& animator = AnimatorControllerMng::Get(animatorKey);
	// Check conditions
	const auto& state = animator.stateMap.at(currentState);
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
	if (!AnimatorControllerMng::Has(animatorKey)) return;
	const auto& animator = AnimatorControllerMng::Get(animatorKey);
	auto& animMng = AnimationMng::Instance();
	const auto& state = animator.stateMap.at(currentState);
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
	if (!AnimatorControllerMng::Has(animatorKey)) return;
	const auto& animator = AnimatorControllerMng::Get(animatorKey);
	if (timer_ms <= 0)
	{
		const auto& animatorState = animator.stateMap.at(currentState);
		auto& animMng = AnimationMng::Instance();
		const auto& animation = animMng.GetAnimation(animatorState.animationList, animatorState.animationState);
		currentDurationID = (currentDurationID - animation.celBaseId + 1) % animation.celCount + animation.celBaseId;
		timer_ms = animMng.GetDuration_ms(currentDurationID);
	}

	timer_ms -= static_cast<int>(deltaTime_s / MathHelper::kMsToSecond);
}

void Animator::Impl::UpdateLoop(float deltaTime_s)
{
	if (!AnimatorControllerMng::Has(animatorKey)) return;
	const auto& animator = AnimatorControllerMng::Get(animatorKey);
	const auto& animatorState = animator.stateMap.at(currentState);
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

void Animator::AddAnimatorController(const std::string& key)
{
	assert(AnimatorControllerMng::Has(key));
	m_impl->animatorKey = key;

	m_impl->currentState = AnimatorControllerMng::Get(key).entryState;
}

void Animator::SetFloat(const std::string& name, float value)
{
	assert(AnimatorControllerMng::Has(m_impl->animatorKey));
	auto& animator = AnimatorControllerMng::Get(m_impl->animatorKey);
	assert(animator.HasParameter(name));
	assert(animator.paramMap.at(name).type == ANIMATOR_PARAMETER_TYPE::FLOAT);
	animator.paramMap.at(name).value = value;
}

void Animator::SetBool(const std::string& name, bool flag)
{
	assert(AnimatorControllerMng::Has(m_impl->animatorKey));
	auto& animator = AnimatorControllerMng::Get(m_impl->animatorKey);
	assert(animator.HasParameter(name));
	assert(animator.paramMap.at(name).type == ANIMATOR_PARAMETER_TYPE::BOOL);
	animator.paramMap.at(name).value = static_cast<float>(flag);
}

void Animator::SetInteger(const std::string& name, int value)
{
	assert(AnimatorControllerMng::Has(m_impl->animatorKey));
	auto& animator = AnimatorControllerMng::Get(m_impl->animatorKey);
	assert(animator.HasParameter(name));
	assert(animator.paramMap.at(name).type == ANIMATOR_PARAMETER_TYPE::INTEGER);
	animator.paramMap.at(name).value = static_cast<float>(value);
}

float Animator::GetFloat(const std::string& name)
{
	assert(AnimatorControllerMng::Has(m_impl->animatorKey));
	const auto& animator = AnimatorControllerMng::Get(m_impl->animatorKey);
	assert(animator.HasParameter(name));
	assert(animator.paramMap.at(name).type == ANIMATOR_PARAMETER_TYPE::FLOAT);
	return animator.paramMap.at(name).value;
}

bool Animator::GetBool(const std::string& name)
{
	assert(AnimatorControllerMng::Has(m_impl->animatorKey));
	const auto& animator = AnimatorControllerMng::Get(m_impl->animatorKey);
	assert(animator.HasParameter(name));
	assert(animator.paramMap.at(name).type == ANIMATOR_PARAMETER_TYPE::BOOL);
	return static_cast<bool>(animator.paramMap.at(name).value);
}

int Animator::GetInteger(const std::string& name)
{
	assert(AnimatorControllerMng::Has(m_impl->animatorKey));
	const auto& animator = AnimatorControllerMng::Get(m_impl->animatorKey);
	assert(animator.HasParameter(name));
	assert(animator.paramMap.at(name).type == ANIMATOR_PARAMETER_TYPE::INTEGER);
	return static_cast<int>(animator.paramMap.at(name).value);
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