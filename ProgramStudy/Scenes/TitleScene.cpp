#include "TitleScene.h"

#include <DxLib.h>

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
		enableChangeScene = true;
}

void TitleScene::Render()
{
	int x, y;
	DxLib::GetGraphSize(m_hBgImage, &x, &y);
	DxLib::DrawGraph(0, 0, m_hBgImage, 0);
}

std::unique_ptr<IScene> TitleScene::ChangeScene(std::unique_ptr<IScene> scene)
{
	enableChangeScene = false;
	scene = std::make_unique<GameScene>();
	scene->Init();
	return scene;
}
