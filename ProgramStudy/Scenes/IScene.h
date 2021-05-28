#pragma once
#include <memory>

class IScene
{
public:
	IScene();
	virtual ~IScene();

	virtual bool Init() = 0;
	virtual void Update(float deltaTime_s) = 0;
	virtual void Render() = 0;
	virtual std::unique_ptr<IScene> ChangeScene(std::unique_ptr<IScene>) = 0;

	bool EnableChangeScene;
};

