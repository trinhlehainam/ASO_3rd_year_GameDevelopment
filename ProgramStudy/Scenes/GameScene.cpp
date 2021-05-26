#include "GameScene.h"

#include <DxLib.h>
#include "../Math/MathHelper.h"

#include "../Geometry/GeometryManager.h"
#include "../Geometry/AABB.h"
#include "../Geometry/Circle.h"
#include "../Geometry/Triangle.h"
#include "../Geometry/ChristmasTree.h"

namespace
{
	constexpr int kScreenWidth = 1024;
	constexpr int kScreenHeight = 768;
}

GameScene::GameScene():m_geoMng(std::make_unique<GeometryManager>(kScreenWidth, kScreenHeight))
{
}

GameScene::~GameScene()
{
}

bool GameScene::Init()
{
	m_geoMng->AddShape(std::make_unique<AABB>(vec2f{ 100.0f,50.0f }, vec2f{ 50.0f,50.0f }, vec2f{ 100.0f,100.0f }, 0xFF0000));
	m_geoMng->AddShape(std::make_unique<Circle>(vec2f{ 50.0f,50.0f }, vec2f{ -100.0f,100.0f }, 50.0f, 0x00FF00));
	m_geoMng->AddShape(std::make_unique<Circle>(vec2f{ 600.0f,100.0f }, vec2f{ -100.0f,100.0f }, 100.0f, 0x00FF00));
	m_geoMng->AddShape(std::make_unique<Circle>(vec2f{ 200.0f,400.0f }, vec2f{ 30.0f, -30.0f }, 150.0f, 0x00FF00));
	m_geoMng->AddShape(std::make_unique<Circle>(vec2f{ 600.0f,500.0f }, vec2f{ 150.0f,150.0f }, 30.0f, 0x00FF00));
	m_geoMng->AddShape(std::make_unique<Triangle>(vec2f{ 30.0f,40.0f }, vec2f{ 10.0f,80.0f }, vec2f{ 100.0f,70.0f }, vec2f{ 50.0f,50.0f }, 0x0000FF));
	m_geoMng->AddShape(std::make_unique<Triangle>(vec2f{ 900.0f,40.0f }, vec2f{ 600.0f,150.0f }, vec2f{ 1000.0f,70.0f }, vec2f{ -50.0f,50.0f }, 0x0000FF));
	m_geoMng->AddShape(std::make_unique<ChristmasTree>(vec2f{ 100.0f,100.0f }, vec2f{ 50.0f,50.0f }));

    return true;
}

void GameScene::Update(float deltaTime_ms)
{
	m_geoMng->Update(deltaTime_ms);
}

void GameScene::Render()
{
	m_geoMng->Render();
}
