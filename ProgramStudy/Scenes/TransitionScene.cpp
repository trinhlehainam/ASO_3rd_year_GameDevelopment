#include "TransitionScene.h"

#include <DxLib.h>

TransitionScene::TransitionScene(std::unique_ptr<IScene> before, std::unique_ptr<IScene> after):
    m_before(std::move(before)), m_after(std::move(after)), 
    m_count(255),
    m_updateFunc(&TransitionScene::UpdateBeforeScene),
    m_renderFunc(&TransitionScene::RenderBeforeScene)
{
    m_after->Init();
}

TransitionScene::~TransitionScene()
{
}

bool TransitionScene::Init()
{
    return true;
}

void TransitionScene::Update(float deltaTime_ms)
{
    (this->*m_updateFunc)(deltaTime_ms);
}

void TransitionScene::Render()
{
    (this->*m_renderFunc)();
}

std::unique_ptr<IScene> TransitionScene::ChangeScene(std::unique_ptr<IScene> scene)
{
    EnableChangeScene = false;
    return std::move(m_after);
}

void TransitionScene::UpdateBeforeScene(float deltaTime)
{
    if (--m_count < 0)
    {
        m_count = 0;
        m_updateFunc = &TransitionScene::UpdateAfterScene;
        m_renderFunc = &TransitionScene::RenderAfterScene;
    }
}

void TransitionScene::UpdateAfterScene(float deltaTime)
{
    if (++m_count > 255)
    {
        m_count = 255;
        EnableChangeScene = true;
    }
}

void TransitionScene::RenderBeforeScene()
{
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, m_count);
    m_before->Render();
    DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void TransitionScene::RenderAfterScene()
{
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, m_count);
    m_after->Render();
    DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}