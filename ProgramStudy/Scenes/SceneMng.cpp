#include "SceneMng.h"

#include "TitleScene.h"

SceneMng& SceneMng::Instance()
{
    static SceneMng sceneMng;
    return sceneMng;
}

SceneMng::SceneMng()
{
    m_scene = std::make_unique<TitleScene>();
}

SceneMng::~SceneMng()
{
}

void SceneMng::Update(float deltaTime_ms)
{
    m_scene->Update(deltaTime_ms);
}

void SceneMng::Render()
{
    m_scene->Render();
}

// Avoid copy and assign
SceneMng::SceneMng(const SceneMng&) {}
SceneMng::SceneMng(SceneMng&&) noexcept {}
void SceneMng::operator=(const SceneMng&) {}
void SceneMng::operator=(SceneMng&&) noexcept {}


