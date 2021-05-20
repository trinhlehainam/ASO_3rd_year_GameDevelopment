#include "GeometryManager.h"

#include "../Math/MathHelper.h"
#include "../Utilities/PhysicsHelper.h"

#include "IShape.h"
#include "AABB.h"
#include "Circle.h"

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
			if (PhysicsHelper::CheckCollision(m_shapes[i].get(), m_shapes[j].get()))
			{
				m_activeDelete = true;
				m_activeAdd = true;
				m_shapes[i]->SetAlive(false);
				m_shapes[j]->SetAlive(false);
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
		m_shapes.emplace_back(std::make_unique<AABB>(vec2f{ MathHelper::randf(800.0f), MathHelper::randf(600.0f) },
													 vec2f{ MathHelper::randf(-150.0f, 150.0f), MathHelper::randf(-150.0f,150.0f) },
													 vec2f{ MathHelper::randf(50.0f,150.0f), MathHelper::randf(50.0f,150.0f) }));
	}
}

void GeometryManager::Render()
{
	for (const auto& shape : m_shapes)
		shape->Draw();
}

