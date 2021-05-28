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

#pragma region Pimpl
class SceneMng::Impl
{
public:
	Impl();
	~Impl() = default;

	void Update();
	void Render();

	float GetDeltaTime_s();

	std::unique_ptr<IScene> scene;
	std::chrono::steady_clock::time_point lastTime;
	float m_deltaTime_s;
};

SceneMng::Impl::Impl() :scene(std::make_unique<TitleScene>()), lastTime(std::chrono::high_resolution_clock::now()) {}

void SceneMng::Impl::Update()
{
	// Update
	m_deltaTime_s = GetDeltaTime_s();
	lastTime = std::chrono::high_resolution_clock::now();
	scene->Update(m_deltaTime_s);
	//

	// Change/Move scene
	if (scene->EnableChangeScene)
		scene = std::move(scene->ChangeScene(std::move(scene)));
	//
}

void SceneMng::Impl::Render()
{
	_dbgStartDraw();
	ClearDrawScreen();
	scene->Render();
	// Show FPS
	DxLib::DrawFormatString(20, 10, GetColor(255, 255, 255), "FPS : %.2f", m_deltaTime_s / MathHelper::kMsToSecond);
	_dbgDraw();
	ScreenFlip();
}

float SceneMng::Impl::GetDeltaTime_s()
{
	return std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - lastTime).count();;
}
#pragma endregion

SceneMng& SceneMng::Instance()
{
    static SceneMng sceneMng;
    return sceneMng;
}

SceneMng::SceneMng():m_impl(std::make_unique<Impl>()) {}

SceneMng::~SceneMng() {}

bool SceneMng::Init()
{
    SetMainWindowText("1916021_TRINH LE HAI NAM");
    ChangeWindowMode(true);
    SetGraphMode(kScreenWidth, kScreenHeight, 32);
    SetDrawScreen(DX_SCREEN_BACK);

    if (DxLib_Init() == -1)
        return false;

    _dbgSetup(kScreenWidth, kScreenHeight, 255);

	m_impl->scene->Init();

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
		m_impl->Update();
		m_impl->Render();
	}
}

// Avoid copy and assign
SceneMng::SceneMng(const SceneMng&) {}
SceneMng::SceneMng(SceneMng&&) noexcept {}
void SceneMng::operator=(const SceneMng&) {}
void SceneMng::operator=(SceneMng&&) noexcept {}
//

