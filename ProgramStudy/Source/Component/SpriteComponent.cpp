#include "SpriteComponent.h"

#include <DxLib.h>
#include <rapidxml.hpp>

#include "../Utilities/StringHelper.h"
#include "../Utilities/ImageMng.h"

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
	auto pImage = pAminationList->first_node();
	for (auto pAttr = pImage->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
	{
		if (strcmp(pAttr->name(), "source") == 0)
		{
			auto& imgMng = ImageMng::Instance();
			imgMng.AddImage(pAttr->value(), key);
			m_textureId = imgMng.GetID(key);
			return true;
		}
	}

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
	DxLib::DrawRectGraphF(transform->Pos.x, transform->Pos.y, 0, 0, 64, 64, m_textureId, 1);
}
