#include "GameScene.h"

#include <DxLib.h>
#include <rapidxml.hpp>

#include "../Math/MathHelper.h"

#include "../AnimationMng.h"
#include "../TileMap.h"
#include "../ImageMng.h"
#include "../Input/Input.h"
#include "../GameObject/Player.h"

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
	auto& animMng = AnimationMng::Instance();
	animMng.LoadAnimationFromXML("Assets/Animations/animation.xml", "knight");

	m_map = std::make_unique<TileMap>("Assets/Map/map.xml", "map");
	m_player = std::make_unique<Player>();

	m_player->Init(INPUT_DEVICE_ID::KEYBOARD);
	RenderToOwnScreen();

    return true;
}

void GameScene::Update(float deltaTime_s)
{
	m_player->Update(deltaTime_s);
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
	m_player->Render();
}

std::unique_ptr<IScene> GameScene::ChangeScene(std::unique_ptr<IScene>)
{
	return std::unique_ptr<IScene>();
}

SCENE_ID GameScene::GetSceneID()
{
	return SCENE_ID::GAME;
}
