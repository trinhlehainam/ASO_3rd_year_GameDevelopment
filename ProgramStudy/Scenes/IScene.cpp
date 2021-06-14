#include "IScene.h"

#include <cassert>

#include <DxLib.h>

IScene::IScene() :
	EnableChangeScene(false),
	m_screenOffsetX(0.0f), m_screenOffsetY(0.0f)
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
	DxLib::DrawGraphF(m_screenOffsetX, m_screenOffsetY, m_screenID, 0);
}
