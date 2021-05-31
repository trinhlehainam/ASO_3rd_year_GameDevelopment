#include "TitleScene.h"

#include <DxLib.h>

#include "TransitionScene.h"
#include "GameScene.h"

TitleScene::TitleScene():m_hBgImage(-1)
{
}

TitleScene::~TitleScene()
{
	DxLib::DeleteGraph(m_hBgImage);
}

bool TitleScene::Init()
{
	m_hBgImage = DxLib::LoadGraph("Assets/Textures/statue.jpg");
	return true;
}

void TitleScene::Update(float deltaTime_s)
{
	if (DxLib::CheckHitKey(KEY_INPUT_SPACE))
		EnableChangeScene = true;
}

void TitleScene::RenderOwnScreen()
{
	DxLib::SetDrawScreen(m_screenID);
	DxLib::ClearDrawScreen();
	int x, y;
	DxLib::GetGraphSize(m_hBgImage, &x, &y);
	DxLib::DrawGraph(0, 0, m_hBgImage, 0);
}

std::unique_ptr<IScene> TitleScene::ChangeScene(std::unique_ptr<IScene> scene)
{
	EnableChangeScene = false;
	scene = std::make_unique<TransitionScene>(std::move(scene), std::make_unique<GameScene>(), TransitionScene::STAGE::CROSS_OVER, TransitionScene::STAGE::FADE_IN);
	scene->Init();
	return scene;
}

SCENE_ID TitleScene::GetSceneID()
{
	return SCENE_ID::TITLE;
}
