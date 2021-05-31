#include "IScene.h"

#include <cassert>

#include <DxLib.h>

IScene::IScene() :
	EnableChangeScene(false)
{
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&m_screenWidth, &m_screenHeight);
	m_screenID = DxLib::MakeScreen(m_screenWidth, m_screenHeight, 1);
	if (m_screenID == -1)
		assert(0);
}

IScene::~IScene()
{
	DeleteGraph(m_screenID);
}

void IScene::Render()
{
	DxLib::DrawGraph(0, 0, m_screenID, 0);
}
