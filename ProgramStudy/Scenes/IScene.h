#pragma once
class IScene
{
public:
	IScene();
	virtual ~IScene();

	virtual void Update(float deltaTime_ms) = 0;
	virtual void Render() = 0;
};

