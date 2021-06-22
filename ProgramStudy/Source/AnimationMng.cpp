#include "AnimationMng.h"

#include <sstream>

#include <rapidxml.hpp>

#include "Utilities/StringHelper.h"
#include "ImageMng.h"

AnimationMng& AnimationMng::Instance()
{
	static AnimationMng instance;
	return instance;
}

bool AnimationMng::LoadAnimationFromXML(const std::string& file, const std::string& key)
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
	m_durations_ms.reserve(m_durations_ms.size() + celCount);

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

		// Add data in duration node to duration container (m_durations_ms)
		auto pDuration = pAnimation->first_node();
		std::stringstream data{ std::move(pDuration->value()) };
		while (!data.eof())
		{
			std::string line;
			data >> line;
			std::stringstream ssLine{ line };
			int duration = 0;
			while (ssLine >> duration)
			{
				m_durations_ms.push_back(duration);

				if (ssLine.peek() == ',')
					ssLine.ignore();
			}

		}
	}

	doc.clear();

	return true;
}

bool AnimationMng::HasAnimation(const std::string& key)
{
	return m_animations.count(key);
}

const Animation& AnimationMng::GetAnimation(const std::string& key) const
{
	return m_animations.at(key);
}

int AnimationMng::GetDuration_ms(int durationIndex) const
{
	return m_durations_ms[durationIndex];
}

AnimationMng::AnimationMng() {}

AnimationMng::~AnimationMng() {}

// Avoid copy and move sematics
AnimationMng::AnimationMng(const AnimationMng&) {}
AnimationMng::AnimationMng(AnimationMng&&) noexcept {}
void AnimationMng::operator = (const AnimationMng&) {}
void AnimationMng::operator = (AnimationMng&&) noexcept {}