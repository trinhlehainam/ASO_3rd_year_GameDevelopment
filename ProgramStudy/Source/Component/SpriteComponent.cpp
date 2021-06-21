#include "SpriteComponent.h"

#include <DxLib.h>
#include <rapidxml.hpp>

#include "../Math/MathHelper.h"
#include "../Utilities/StringHelper.h"
#include "../ImageMng.h"

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

bool SpriteComponent::LoadAnimationFromXML(const std::string& file, const std::string& key)
{
	if (m_animations.count(key)) return false;

	rapidxml::xml_document<> doc;
	auto content = StringHelper::LoadFileToStringBuffer(file);
	doc.parse<0>(&content[0]);

	auto pAminationList = doc.first_node();
	int celWidth = 0;
	int celHeight = 0;
	int celCount = 0;
	int texColumns = 0;
	for (auto pAttr = pAminationList->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "celwidth") == 0)
			celWidth = std::atoi(pAttr->value());
		else if (strcmp(pAttr->name(), "celheight") == 0)
			celHeight = std::atoi(pAttr->value());
		else if (strcmp(pAttr->name(), "celcount") == 0)
			celCount = std::atoi(pAttr->value());
		else if (strcmp(pAttr->name(), "columns") == 0)
			texColumns = std::atoi(pAttr->value());
	}
	m_durations_ms.reserve(celCount);

	auto pImage = pAminationList->first_node("image");
	for (auto pAttr = pImage->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "source") == 0)
		{
			auto& imgMng = ImageMng::Instance();
			imgMng.AddImage(pAttr->value(), key);
		}
	}
	
	for (auto pAnimation = pAminationList->first_node("animation"); pAnimation; pAnimation = pAnimation->next_sibling())
	{
		std::string animKey;
		for (auto pAttr = pAnimation->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			if (strcmp(pAttr->name(), "name") == 0)
			{
				animKey = key + "_" + pAttr->value();
				m_animations[animKey].texId = ImageMng::Instance().GetID(key);
				m_animations[animKey].texColumns = texColumns;
				m_animations[animKey].celWidth = celWidth;
				m_animations[animKey].celHeight = celHeight;
			}
			else if (strcmp(pAttr->name(), "id") == 0)
				m_animations[animKey].celBaseId = std::atoi(pAttr->value());
			else if (strcmp(pAttr->name(), "count") == 0)
				m_animations[animKey].celCount = std::atoi(pAttr->value());
			else if (strcmp(pAttr->name(), "loop") == 0)
				m_animations[animKey].loop = std::atoi(pAttr->value());
		}
	}
	
	for (int i = 0; i < celCount; ++i)
		m_durations_ms.push_back(100);

	doc.clear();

	return true;
}

bool SpriteComponent::Play(const std::string& animKey, const std::string& state)
{
	std::string key{ animKey + "_" + state };
	if (!m_animations.count(key)) return false;
	if (IsPlaying(animKey, state)) return true;
	m_currentAnimKey = std::move(key);
	m_currentDurationId = m_animations[m_currentAnimKey].celBaseId;
	m_timer_ms = m_durations_ms[m_currentDurationId];
	switch (m_animations[m_currentAnimKey].loop)
	{
	case -1:
		m_updateFunc = &SpriteComponent::UpdateInfinite;
		break;
	case 0:
		m_updateFunc = &SpriteComponent::UpdateOnce;
		break;
	default:
		m_updateFunc = &SpriteComponent::UpdateLoop;
		m_loopCount = m_animations[m_currentAnimKey].loop;
		break;
	}
	return true;
}

bool SpriteComponent::IsPlaying(const std::string& animKey, const std::string& state)
{
	std::string key{ animKey + "_" + state };
	return m_currentAnimKey == key;
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
		const auto& currentAnim = m_animations[m_currentAnimKey];
		m_currentDurationId = (m_currentDurationId - currentAnim.celBaseId + 1) % currentAnim.celCount + currentAnim.celBaseId;
		m_timer_ms = m_durations_ms[m_currentDurationId];
	}

	m_timer_ms -= static_cast<int>(deltaTime_s / MathHelper::kMsToSecond);
}

void SpriteComponent::UpdateLoop(float deltaTime_s)
{
	if (m_timer_ms <= 0)
	{
		++m_currentDurationId;
		m_timer_ms = m_durations_ms[m_currentDurationId];
	}

	const auto& currentAnim = m_animations[m_currentAnimKey];
	if (m_currentDurationId >= (currentAnim.celBaseId + currentAnim.celCount))
	{
		--m_loopCount;
		m_currentDurationId = currentAnim.celBaseId;
		m_timer_ms = m_durations_ms[m_currentDurationId];
	}

	if (m_loopCount < 0)
	{
		m_updateFunc = &SpriteComponent::UpdateSleep;
		return;
	}

	m_timer_ms -= static_cast<int>(deltaTime_s / MathHelper::kMsToSecond);
}

void SpriteComponent::UpdateOnce(float deltaTime_s)
{
	if (m_timer_ms <= 0)
	{
		++m_currentDurationId;
		m_timer_ms = m_durations_ms[m_currentDurationId];
	}

	const auto& currentAnim = m_animations[m_currentAnimKey];
	if (m_currentDurationId >= (currentAnim.celBaseId + currentAnim.celCount))
	{
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
	const auto& transform = m_transform.lock();
	const auto& currentAnim = m_animations[m_currentAnimKey];
	auto sourceX = (m_currentDurationId % currentAnim.texColumns) * currentAnim.celWidth;
	auto sourceY = (m_currentDurationId / currentAnim.texColumns) * currentAnim.celHeight;

	DxLib::DrawRectExtendGraphF(transform->Pos.x, transform->Pos.y, 
		(transform->Pos.x + transform->Size.x) * transform->Scale, 
		(transform->Pos.y + transform->Size.y) * transform->Scale,
		sourceX, sourceY, currentAnim.celWidth, currentAnim.celHeight, 
		m_animations[m_currentAnimKey].texId, 1);

}