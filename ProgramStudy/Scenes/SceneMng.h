#pragma once
#include <memory>

class IScene;

// Singleton
class SceneMng
{
public:
	static SceneMng& Instance();
	~SceneMng();

	void Update(float deltaTime_ms);
	void Render();
private:
	SceneMng();
private:
	// Don't allow copy and move semantics
	//SceneMng(const SceneMng&);
	//SceneMng(SceneMng&&) noexcept;
	//SceneMng& operator = (const SceneMng&);
	//SceneMng& operator = (SceneMng&&) noexcept;
	//
private:
	std::unique_ptr<IScene> m_scene;
};

