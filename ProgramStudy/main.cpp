#include <memory>
#include <vector>
#include <chrono>
#include <algorithm>

#include <Dxlib.h>

#include "_debug/_DebugDispOut.h"
#include "Math/MathHelper.h"

#include "Geometry/GeometryManager.h"
#include "Geometry/AABB.h"
#include "Geometry/Circle.h"
#include "Geometry/Triangle.h"
#include "Geometry/ChristmasTree.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("1916021_TRINH LE HAI NAM");
	ChangeWindowMode(true);
	SetGraphMode(800, 600, 32);
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)
		return -1;

	_dbgSetup(800, 600, 255);

	GeometryManager geoMng(800,600);
	geoMng.AddShape(std::make_unique<AABB>(vec2f{ 100.0f,50.0f }, vec2f{ 50.0f,50.0f }, vec2f{ 100.0f,100.0f }, 0xFF0000));
	geoMng.AddShape(std::make_unique<Circle>(vec2f{ 50.0f,50.0f }, vec2f{ -100.0f,100.0f }, 50.0f, 0x00FF00));
	geoMng.AddShape(std::make_unique<Circle>(vec2f{ 600.0f,100.0f }, vec2f{ -100.0f,100.0f }, 100.0f, 0x00FF00));
	geoMng.AddShape(std::make_unique<Circle>(vec2f{ 200.0f,400.0f }, vec2f{ 30.0f, -30.0f }, 150.0f, 0x00FF00));
	geoMng.AddShape(std::make_unique<Circle>(vec2f{ 600.0f,500.0f }, vec2f{ 150.0f,150.0f }, 30.0f, 0x00FF00));
	geoMng.AddShape(std::make_unique<Triangle>(vec2f{ 30.0f,40.0f }, vec2f{ 10.0f,80.0f }, vec2f{ 100.0f,70.0f }, vec2f{ 150.0f,150.0f }, 0x0000FF));
	geoMng.AddShape(std::make_unique<ChristmasTree>(vec2f{ 100.0f,100.0f }, vec2f{ 50.0f,50.0f }));

	auto currentTime_chroro = std::chrono::high_resolution_clock::now();
	auto lastTime_chrono = currentTime_chroro;

	while (ProcessMessage() != -1 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgStartDraw();

		lastTime_chrono = currentTime_chroro;
		currentTime_chroro = std::chrono::high_resolution_clock::now();

		float deltaTime_ms = std::chrono::duration<float, std::chrono::seconds::period>(currentTime_chroro - lastTime_chrono).count();
		geoMng.Update(deltaTime_ms);

		ClearDrawScreen();
		geoMng.Render();

		DxLib::DrawFormatString(20, 10, GetColor(255, 255, 255), "FPS : %.2f", deltaTime_ms / MathHelper::kMsToSecond);

		_dbgDraw();
		ScreenFlip();
	}

	DxLib_End();

	return 0;
}