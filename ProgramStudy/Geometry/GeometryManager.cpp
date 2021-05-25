#include "GeometryManager.h"

#include "../Math/MathHelper.h"

#include "IShape.h"
#include "AABB.h"
#include "Circle.h"
#include "Triangle.h"

namespace
{
	bool isOverlap(const IShape* a, const IShape* b)
	{

		if (a->Type == IShape::TYPE::CIRCLE)
		{
			auto circleA = dynamic_cast<const Circle*>(a);

			if (b->Type == IShape::TYPE::CIRCLE)
			{
				auto circleB = dynamic_cast<const Circle*>(b);

				if (MathHelper::isOverlap(*circleA, *circleB))
					return true;
			}
			if (b->Type == IShape::TYPE::AABB)
			{
				auto rectB = dynamic_cast<const AABB*>(b);

				if (MathHelper::isOverlap(*circleA, *rectB))
					return true;
			}
		}
		else if (a->Type == IShape::TYPE::AABB)
		{
			auto rectA = dynamic_cast<const AABB*>(a);

			if (b->Type == IShape::TYPE::AABB)
			{
				auto rectB = dynamic_cast<const AABB*>(b);

				if (MathHelper::isOverlap(*rectA, *rectB))
					return true;
			}
			if (b->Type == IShape::TYPE::CIRCLE)
			{
				auto circleB = dynamic_cast<const Circle*>(b);

				if (MathHelper::isOverlap(*rectA, *circleB))
					return true;
			}
		}
		else if (a->Type == IShape::TYPE::TRIANGLE)
		{
			auto triA = dynamic_cast<const Triangle*>(a);

			if (b->Type == IShape::TYPE::TRIANGLE)
			{
				auto triB = dynamic_cast<const Triangle*>(b);

				if (MathHelper::isOverlap(*triA, *triB))
					return true;
			}
		}

		return false;
	}
}

GeometryManager::GeometryManager(float screenWidth, float screenHeight):
	m_screenWidth(screenWidth), m_screenHeight(screenHeight),
	m_activeDelete(false),m_activeAdd(false)
{
	m_shapes.reserve(100);
}

GeometryManager::~GeometryManager()
{
}

GeometryManager::GeometryManager(const GeometryManager&) {}
GeometryManager::GeometryManager(GeometryManager&&) noexcept {}
void GeometryManager::operator=(const GeometryManager&) {}
void GeometryManager::operator=(GeometryManager&&) noexcept {}

void GeometryManager::AddShape(std::unique_ptr<IShape> shape)
{
	m_shapes.push_back(std::move(shape));
}

void GeometryManager::Update(float deltaTime_ms)
{
	for (const auto& shape : m_shapes)
		shape->Update(deltaTime_ms);

	for (const auto& shape : m_shapes)
		shape->ConstrainPosition(m_screenWidth, m_screenHeight);

	auto num_shapes = m_shapes.size();
	for (int i = 0; i < num_shapes; ++i)
	{
		for (int j = 0; j < num_shapes; ++j)
		{
			if (i == j)
				continue;
			if (isOverlap(m_shapes[i].get(), m_shapes[j].get()))
			{
				m_activeDelete = true;
				m_activeAdd = true;
				m_shapes[i]->SpecialAction(m_shapes);
				m_shapes[j]->SpecialAction(m_shapes);
			}
		}
	}

	auto deleteFunc = [](const std::unique_ptr<IShape>& shape)->bool { return !shape->IsAlive; };
	if (m_activeDelete)
	{
		m_activeDelete = false;
		std::erase_if(m_shapes, deleteFunc);
	}	

	if (m_activeAdd)
	{
		m_activeAdd = false;
	}
}

void GeometryManager::Render()
{
	for (const auto& shape : m_shapes)
		shape->Draw();
}

