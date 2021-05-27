#include "TitleScene.h"

#include <DxLib.h>

#include "TransitionScene.h"
#include "GameScene.h"

TitleScene::TitleScene()
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

void TitleScene::Update(float deltaTime_ms)
{
	if (DxLib::CheckHitKey(KEY_INPUT_SPACE))
		EnableChangeScene = true;
}

void TitleScene::Render()
{
	int x, y;
	DxLib::GetGraphSize(m_hBgImage, &x, &y);
	DxLib::DrawGraph(0, 0, m_hBgImage, 0);
}

std::unique_ptr<IScene> TitleScene::ChangeScene(std::unique_ptr<IScene> scene)
{
	EnableChangeScene = false;
	scene = std::make_unique<TransitionScene>(std::move(scene), std::make_unique<GameScene>());
	scene->Init();
	return scene;
}
