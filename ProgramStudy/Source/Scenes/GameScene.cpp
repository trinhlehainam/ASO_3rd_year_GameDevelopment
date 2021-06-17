#include "GameScene.h"

#include <DxLib.h>
#include <rapidxml.hpp>

#include "../Math/MathHelper.h"

#include "../Utilities/TileMap.h"
#include "../Utilities/ImageMng.h"
#include "../GameObject/Entity.h"
#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Input/Input.h"

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
	m_entity = std::make_shared<Entity>(INPUT_DEVICE_ID::KEYBOARD);
	// Screen is empty -> Init draw screen
	m_entity->AddComponent<TransformComponent>(m_entity, vec2f{ 100.0f,100.0f }, vec2f{ 100.0f,100.0f }, 1.0f);
	m_entity->AddComponent<SpriteComponent>(m_entity);
	auto sprite = m_entity->GetComponent<SpriteComponent>();
	sprite->LoadAnimationFromXML("Assets/Animations/animation.xml");
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
