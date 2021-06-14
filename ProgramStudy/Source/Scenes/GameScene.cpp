#include "GameScene.h"

#include <DxLib.h>
#include <rapidxml.hpp>

#include "../Math/MathHelper.h"

#include "../Utilities/TileMap.h"
#include "../Utilities/ImageMng.h"
#include "../Common/Entity.h"

namespace
{
	constexpr int kScreenWidth = 1024;
	constexpr int kScreenHeight = 768;
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::Init()
{
	m_map = std::make_unique<TileMap>("Assets/Map/map.xml");
	m_entity = std::make_unique<Entity>(vec2f{ 100.0f,100.0f }, vec2f{ 100.0f,100.0f });
	// Screen is empty -> Init draw screen
	RenderToOwnScreen();

    return true;
}

void GameScene::Update(float deltaTime_s)
{
	m_entity->Update(deltaTime_s);
}

void GameScene::Render()
{
	IScene::Render();
}

void GameScene::RenderToOwnScreen()
{
	DxLib::SetDrawScreen(m_screenID);
	DxLib::ClearDrawScreen();

	m_map->Render();
	m_entity->Render();
}

std::unique_ptr<IScene> GameScene::ChangeScene(std::unique_ptr<IScene>)
{
	return std::unique_ptr<IScene>();
}

SCENE_ID GameScene::GetSceneID()
{
	return SCENE_ID::GAME;
}
