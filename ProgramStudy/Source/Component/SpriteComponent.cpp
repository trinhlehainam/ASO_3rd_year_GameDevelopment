#include "SpriteComponent.h"

#include <DxLib.h>
#include <rapidxml.hpp>

#include "../Utilities/StringHelper.h"
#include "../ImageMng.h"

#include "../GameObject/Entity.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(const std::shared_ptr<Entity>& owner):
	IComponent(owner), m_transform(owner->GetComponent<TransformComponent>())
{

}

SpriteComponent::~SpriteComponent()
{
}

bool SpriteComponent::LoadAnimationFromXML(const std::string& file, const std::string& key)
{
	if (m_animations.count(key)) return false;
	auto& animationInfo = m_animations[key];

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

	auto pImage = pAminationList->first_node("image");
	for (auto pAttr = pImage->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "source") == 0)
		{
			auto& imgMng = ImageMng::Instance();
			imgMng.AddImage(pAttr->value(), key);
			animationInfo.texId = imgMng.GetID(key);
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


	m_currentAnim = key;

	doc.clear();

	return true;
}

void SpriteComponent::Init()
{
}

void SpriteComponent::Update(float deltaTime_s)
{
}

void SpriteComponent::Render()
{
	const auto& transform = m_transform.lock();
	DxLib::DrawRectGraphF(transform->Pos.x, transform->Pos.y, 0, 0, 64, 64, m_animations[m_currentAnim].texId, 1);
}
