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
	rapidxml::xml_document<> doc;
	auto content = StringHelper::LoadFileToStringBuffer(file);
	doc.parse<0>(&content[0]);

	auto pAminationList = doc.first_node();
	int columns = 0;
	for (auto pAttr = pAminationList->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "celwidth") == 0)
			m_animations[key].celWidth = std::atoi(pAttr->value());
		else if (strcmp(pAttr->name(), "celheight") == 0)
			m_animations[key].celHeight = std::atoi(pAttr->value());
		else if (strcmp(pAttr->name(), "columns") == 0)
			columns = std::atoi(pAttr->value());
	}

	auto pImage = pAminationList->first_node("image");
	for (auto pAttr = pImage->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "source") == 0)
		{
			auto& imgMng = ImageMng::Instance();
			imgMng.AddImage(pAttr->value(), key);
			m_animations[key].texId = imgMng.GetID(key);
		}
		else if (strcmp(pAttr->name(), "width") == 0)
		{
			m_animations[key].texWidth = std::atoi(pAttr->value());
		}
		else if (strcmp(pAttr->name(), "height") == 0)
		{
			m_animations[key].texHeight = std::atoi(pAttr->value());
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
