#include "IScene.h"

#include <DxLib.h>

IScene::IScene() :
	EnableChangeScene(false), 
	m_screenWidth(1024), m_screenHeight(768),
	m_screenID(DxLib::MakeScreen(m_screenWidth, m_screenHeight))
{
}

IScene::~IScene()
{
}
