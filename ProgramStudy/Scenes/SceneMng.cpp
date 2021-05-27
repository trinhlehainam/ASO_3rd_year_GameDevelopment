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

	float GetDeltaTime_ms();
	void UpdateClock();

	std::unique_ptr<IScene> scene;
	std::chrono::steady_clock::time_point lastTime;
};

SceneMng::Impl::Impl() :scene(std::make_unique<TitleScene>()), lastTime(std::chrono::high_resolution_clock::now()) {}

float SceneMng::Impl::GetDeltaTime_ms()
{
	return std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - lastTime).count();;
}
void SceneMng::Impl::UpdateClock()
{
	lastTime = std::chrono::high_resolution_clock::now();
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
		_dbgStartDraw();

		float deltaTime_ms = m_impl->GetDeltaTime_ms();
		m_impl->UpdateClock();

		m_impl->scene->Update(deltaTime_ms);

		if (m_impl->scene->EnableChangeScene)
			m_impl->scene = std::move(m_impl->scene->ChangeScene(std::move(m_impl->scene)));

		ClearDrawScreen();

		m_impl->scene->Render();

		// Show FPS
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
//

