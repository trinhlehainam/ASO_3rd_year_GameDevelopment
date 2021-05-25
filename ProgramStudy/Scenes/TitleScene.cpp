#include "TitleScene.h"

#include <DxLib.h>

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
}

void TitleScene::Render()
{
	int x, y;
	DxLib::GetGraphSize(m_hBgImage, &x, &y);
	DxLib::DrawGraph(0, 0, m_hBgImage, 0);
}
