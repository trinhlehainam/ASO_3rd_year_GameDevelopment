#include "ChristmasTree.h"

#include "AABB.h"
#include "Triangle.h"

ChristmasTree::ChristmasTree(vec2f pos, vec2f speed):IShape(IShape::TYPE::CHRISTMAS_TREE,pos,speed)
{
	AABB trunk{ vec2f{pos.x - 20.0f,pos.y + 20.0f},Speed,vec2f{40.0f,170.0f} };
	Elements.push_back(std::make_unique<AABB>(std::move(trunk)));

	float leaf_base_y = pos.y + 20.0f;
	vec2f leaf_offset{ 60.0f,40.0f };
	Triangle leaf{ vec2f{pos.x,pos.y - 20.0f}, vec2f{pos.x - leaf_offset.x,pos.y + 40.0f},  vec2f{pos.x + leaf_offset.x,pos.y + 40.0f}, Speed };
	for (int i = 0; i < 4; ++i)
	{
		Elements.push_back(std::make_unique<Triangle>(leaf));
		leaf.Points[0].y += leaf_offset.y;
		leaf.Points[1].y += leaf_offset.y;
		leaf.Points[2].y += leaf_offset.y;
	}
}

ChristmasTree::~ChristmasTree()
{
}

void ChristmasTree::ConstrainPosition(float width, float height)
{
}

void ChristmasTree::Update(float deltaTime_ms)
{
	for (const auto& element : Elements)
		element->Update(deltaTime_ms);
}

void ChristmasTree::SpecialAction()
{
}

void ChristmasTree::Draw()
{
	for (const auto& element : Elements)
		element->Draw();
}

void ChristmasTree::Draw(float scale)
{
}
