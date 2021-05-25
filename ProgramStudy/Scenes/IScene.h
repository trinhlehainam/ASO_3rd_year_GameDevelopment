#pragma once
class IScene
{
public:
	IScene();
	virtual ~IScene();

	virtual bool Init() = 0;
	virtual void Update(float deltaTime_ms) = 0;
	virtual void Render() = 0;
};

