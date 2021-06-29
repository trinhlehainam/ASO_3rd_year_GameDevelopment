#include "GameScene.h"

#include <DxLib.h>
#include <rapidxml.hpp>

#include "../Math/MathHelper.h"

#include "../Systems/AnimationMng.h"
#include "../Systems/EntityMng.h"
#include "../TileMap.h"
#include "../Systems/ImageMng.h"
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
	animMng.LoadAnimationFromXML("Assets/Animations/animation.xml");

	m_entityMng = std::make_shared<EntityMng>();
	m_map = std::make_shared<TileMap>("Assets/Map/map.xml", "map");
	m_player = std::make_shared<Player>();

	m_player->Init(INPUT_DEVICE_ID::KEYBOARD);
	m_entityMng->AddEntity(m_player->GetEntity());
	RenderToOwnScreen();

    return true;
}

void GameScene::Update(float deltaTime_s)
{
	m_player->Update(deltaTime_s);
	m_entityMng->Update(deltaTime_s);
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
	m_entityMng->Render();
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
