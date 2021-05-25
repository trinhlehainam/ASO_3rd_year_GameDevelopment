#include "SceneMng.h"

#include <chrono>

#include <DxLib.h>
#include "../_debug/_DebugDispOut.h"
#include "../Math/MathHelper.h"

#include "TitleScene.h"
#include "GameScene.h"

namespace
{
	constexpr int kScreenWidth = 1024;
	constexpr int kScreenHeight = 768;
}

SceneMng& SceneMng::Instance()
{
    static SceneMng sceneMng;
    return sceneMng;
}

SceneMng::SceneMng():
	m_scene(std::make_unique<TitleScene>()), 
	m_lastTime(std::chrono::high_resolution_clock::now())
{
}

SceneMng::~SceneMng()
{
}

bool SceneMng::SysInit()
{
    SetMainWindowText("1916021_TRINH LE HAI NAM");
    ChangeWindowMode(true);
    SetGraphMode(kScreenWidth, kScreenHeight, 32);
    SetDrawScreen(DX_SCREEN_BACK);

    if (DxLib_Init() == -1)
        return false;

    _dbgSetup(kScreenWidth, kScreenHeight, 255);

	m_scene->Init();

    return true;
}

void SceneMng::Exit()
{
    DxLib_End();
}

void SceneMng::Run()
{

	while (ProcessMessage() != -1 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgStartDraw();

		float deltaTime_ms = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - m_lastTime).count();
		m_lastTime = std::chrono::high_resolution_clock::now();

		m_scene->Update(deltaTime_ms);

		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			m_scene = std::make_unique<GameScene>();
			m_scene->Init();
		}

		ClearDrawScreen();

		m_scene->Render();

		DxLib::DrawFormatString(20, 10, GetColor(255, 255, 255), "FPS : %.2f", deltaTime_ms / MathHelper::kMsToSecond);

		_dbgDraw();
		ScreenFlip();
	}
}

// Avoid copy and assign
SceneMng::SceneMng(const SceneMng&) {}
SceneMng::SceneMng(SceneMng&&) noexcept {}
void SceneMng::operator=(const SceneMng&) {}
void SceneMng::operator=(SceneMng&&) noexcept {}


