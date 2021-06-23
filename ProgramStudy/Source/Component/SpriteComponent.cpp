#include "SpriteComponent.h"

#include <DxLib.h>

#include "../_debug/_DebugConOut.h"
#include "../Math/MathHelper.h"

#include "../Systems/AnimationMng.h"
#include "../GameObject/Entity.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(const std::shared_ptr<Entity>& owner):
	IComponent(owner), m_transform(owner->GetComponent<TransformComponent>()),
	m_currentDurationId(0), m_timer_ms(0), m_loopCount(0),
	m_updateFunc(&SpriteComponent::UpdateSleep)
{
}

SpriteComponent::~SpriteComponent()
{
}

bool SpriteComponent::Play(const std::string& listKey, const std::string& state)
{
	auto& AnimMng = AnimationMng::Instance();
	std::string key{ listKey + "_" + state };

	if (!AnimMng.HasAnimation(key)) return false;
	if (IsPlaying(listKey, state)) return true;

	m_animKey = std::move(key);
	auto& animation = AnimMng.GetAnimation(m_animKey);
	m_currentDurationId = animation.celBaseId;
	m_timer_ms = AnimMng.GetDuration_ms(m_currentDurationId);

	switch (animation.loop)
	{
	case -1:
		m_updateFunc = &SpriteComponent::UpdateInfinite;
		break;
	default:
		m_updateFunc = &SpriteComponent::UpdateLoop;
		m_loopCount = animation.loop;
		break;
	}
	return true;
}

bool SpriteComponent::IsPlaying(const std::string& listKey, const std::string& state)
{
	std::string key{ listKey + "_" + state };
	return m_animKey == key;
}

void SpriteComponent::Init()
{
}

void SpriteComponent::Update(float deltaTime_s)
{
	(this->*m_updateFunc)(deltaTime_s);
}

void SpriteComponent::UpdateInfinite(float deltaTime_s)
{
	if (m_timer_ms <= 0)
	{
		auto& animMng = AnimationMng::Instance();
		const auto& animtion = animMng.GetAnimation(m_animKey);
		m_currentDurationId = (m_currentDurationId - animtion.celBaseId + 1) % animtion.celCount + animtion.celBaseId;
		m_timer_ms = animMng.GetDuration_ms(m_currentDurationId);
	}

	m_timer_ms -= static_cast<int>(deltaTime_s / MathHelper::kMsToSecond);
}

void SpriteComponent::UpdateLoop(float deltaTime_s)
{
	auto& animMng = AnimationMng::Instance();
	const auto& animation = animMng.GetAnimation(m_animKey);;

	if (m_timer_ms <= 0)
	{
		++m_currentDurationId;
		m_timer_ms = animMng.GetDuration_ms(m_currentDurationId);
	}

	if (m_currentDurationId >= (animation.celBaseId + animation.celCount))
	{
		--m_loopCount;
		m_currentDurationId = animation.celBaseId;
		m_timer_ms = animMng.GetDuration_ms(m_currentDurationId);
	}

	if (m_loopCount < 0)
	{
		// Last durationId of current animation
		m_currentDurationId = animation.celBaseId + animation.celCount - 1;
		m_updateFunc = &SpriteComponent::UpdateSleep;
		return;
	}

	m_timer_ms -= static_cast<int>(deltaTime_s / MathHelper::kMsToSecond);
}

void SpriteComponent::UpdateSleep(float deltaTime_s)
{

}

void SpriteComponent::Render()
{
	auto& animMng = AnimationMng::Instance();
	const auto& transform = m_transform.lock();
	const auto& animation = animMng.GetAnimation(m_animKey);
	auto sourceX = (m_currentDurationId % animation.texColumns) * animation.celWidth;
	auto sourceY = (m_currentDurationId / animation.texColumns) * animation.celHeight;

	DxLib::DrawRectExtendGraphF(transform->Pos.x, transform->Pos.y, 
		(transform->Pos.x + transform->Size.x) * transform->Scale, 
		(transform->Pos.y + transform->Size.y) * transform->Scale,
		sourceX, sourceY, animation.celWidth, animation.celHeight, 
		animation.texId, 1);
}