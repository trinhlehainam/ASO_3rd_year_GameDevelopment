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
	SceneMng(const SceneMng&);
	SceneMng(SceneMng&&) noexcept;
	void operator = (const SceneMng&);
	void operator = (SceneMng&&) noexcept;
	//
private:
	std::unique_ptr<IScene> m_scene;
};

