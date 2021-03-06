#include "TitleScene.h"

#include <DxLib.h>

#include "../Systems/ImageMng.h"

#include "TransitionScene.h"
#include "GameScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	ImageMng::DeleteImage("title");
}

bool TitleScene::Init()
{
	ImageMng::AddImage("Assets/Textures/statue.jpg", "title");

	return true;
}

void TitleScene::Update(float deltaTime_s)
{
	if (DxLib::CheckHitKey(KEY_INPUT_SPACE))
		EnableChangeScene = true;
}

void TitleScene::RenderToOwnScreen()
{
	DxLib::SetDrawScreen(m_screenID);
	DxLib::ClearDrawScreen();

	auto imageId = ImageMng::GetID("title");
	int x, y;
	DxLib::GetGraphSize(imageId, &x, &y);
	DxLib::DrawGraph(0, 0, imageId, 0);
}

std::unique_ptr<IScene> TitleScene::ChangeScene(std::unique_ptr<IScene> scene)
{
	EnableChangeScene = false;
	scene = std::make_unique<TransitionScene>(std::move(scene), std::make_unique<GameScene>(), TransitionScene::STAGE::CROSS_OVER, TransitionScene::STAGE::FADE_IN);
	scene->Init();
	return scene;
}

SCENE_ID TitleScene::GetSceneID()
{
	return SCENE_ID::TITLE;
}
