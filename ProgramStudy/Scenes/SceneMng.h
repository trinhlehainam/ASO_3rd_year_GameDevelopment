#pragma once
#include <memory>
#include <chrono>

class IScene;
class GeometryManager;

// Singleton
class SceneMng
{
public:
	static SceneMng& Instance();
	~SceneMng();

	bool SysInit();
	void Run();
	void Exit();

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
	std::chrono::steady_clock::time_point m_lastTime;
};

