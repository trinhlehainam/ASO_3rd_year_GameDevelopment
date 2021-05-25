#include <DxLib.h>

#include "Scenes/SceneMng.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	auto& sceneMng = SceneMng::Instance();
	sceneMng.SysInit();
	sceneMng.Run();
	sceneMng.Exit();

	return 0;
}