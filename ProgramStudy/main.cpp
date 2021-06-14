#include <DxLib.h>

#include "Source/Scenes/SceneMng.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	auto& sceneMng = SceneMng::Instance();
	sceneMng.Init();
	sceneMng.Run();
	sceneMng.Exit();

	return 0;
}