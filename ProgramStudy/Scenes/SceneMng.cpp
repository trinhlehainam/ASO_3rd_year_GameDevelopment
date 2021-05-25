#include "SceneMng.h"

#include <chrono>

#include <DxLib.h>
#include "../_debug/_DebugDispOut.h"
#include "../Math/MathHelper.h"

#include "TitleScene.h"

#include "../Geometry/GeometryManager.h"
#include "../Geometry/AABB.h"
#include "../Geometry/Circle.h"
#include "../Geometry/Triangle.h"
#include "../Geometry/ChristmasTree.h"

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
	m_geoMng(std::make_unique<GeometryManager>(kScreenWidth, kScreenHeight)),
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

	m_geoMng->AddShape(std::make_unique<AABB>(vec2f{ 100.0f,50.0f }, vec2f{ 50.0f,50.0f }, vec2f{ 100.0f,100.0f }, 0xFF0000));
	m_geoMng->AddShape(std::make_unique<Circle>(vec2f{ 50.0f,50.0f }, vec2f{ -100.0f,100.0f }, 50.0f, 0x00FF00));
	m_geoMng->AddShape(std::make_unique<Circle>(vec2f{ 600.0f,100.0f }, vec2f{ -100.0f,100.0f }, 100.0f, 0x00FF00));
	m_geoMng->AddShape(std::make_unique<Circle>(vec2f{ 200.0f,400.0f }, vec2f{ 30.0f, -30.0f }, 150.0f, 0x00FF00));
	m_geoMng->AddShape(std::make_unique<Circle>(vec2f{ 600.0f,500.0f }, vec2f{ 150.0f,150.0f }, 30.0f, 0x00FF00));
	m_geoMng->AddShape(std::make_unique<Triangle>(vec2f{ 30.0f,40.0f }, vec2f{ 10.0f,80.0f }, vec2f{ 100.0f,70.0f }, vec2f{ 150.0f,150.0f }, 0x0000FF));
	m_geoMng->AddShape(std::make_unique<Triangle>(vec2f{ 900.0f,40.0f }, vec2f{ 600.0f,150.0f }, vec2f{ 1000.0f,70.0f }, vec2f{ -150.0f,150.0f }, 0x0000FF));
	m_geoMng->AddShape(std::make_unique<ChristmasTree>(vec2f{ 100.0f,100.0f }, vec2f{ 50.0f,50.0f }));

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
		m_geoMng->Update(deltaTime_ms);

		ClearDrawScreen();

		auto minMax = MathHelper::randf(100.0f, 600.0f);
		auto num = MathHelper::randf(50.0f);

		m_scene->Render();
		m_geoMng->Render();

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


