#pragma once
#include <memory>

enum class SCENE_ID
{
	TITLE,
	GAME,
	TRANSITION
};

class IScene
{
public:
	IScene();
	virtual ~IScene();

	virtual bool Init() = 0;
	virtual void Update(float deltaTime_s) = 0;
	virtual void Render();
	virtual void RenderToOwnScreen() = 0;
	virtual std::unique_ptr<IScene> ChangeScene(std::unique_ptr<IScene>) = 0;
	virtual SCENE_ID GetSceneID() = 0;

	bool EnableChangeScene;

protected:
	float m_screenOffsetX, m_screenOffsetY;
	int m_screenWidth, m_screenHeight;
	int m_screenID;
};

