#include "TransitionScene.h"

#include <DxLib.h>

namespace
{
    constexpr float kTransitionTime = 3.0f;
}

TransitionScene::TransitionScene(std::unique_ptr<IScene> before, std::unique_ptr<IScene> after):
    m_before(std::move(before)), m_after(std::move(after)), 
    m_stageBefore(STAGE::CROSS_OVER), m_stageAfter(STAGE::CROSS_OVER),
    m_timer_s(kTransitionTime),
    m_updateFunc(&TransitionScene::UpdateCrossOverScene),
    m_renderFunc(&TransitionScene::RenderCrossOverScene)
{
    m_after->Init();
}

TransitionScene::TransitionScene(std::unique_ptr<IScene> before, std::unique_ptr<IScene> after, STAGE stage):
    m_before(std::move(before)), m_after(std::move(after)),
    m_stageBefore(stage), m_stageAfter(stage),
    m_timer_s(kTransitionTime),
    m_updateFunc(&TransitionScene::UpdateBeforeScene),
    m_renderFunc(&TransitionScene::RenderBeforeScene)
{
     m_after->Init();
}

TransitionScene::TransitionScene(std::unique_ptr<IScene> before, std::unique_ptr<IScene> after, STAGE stageBefore, STAGE stageAfter):
    m_before(std::move(before)), m_after(std::move(after)),
    m_stageBefore(stageBefore), m_stageAfter(stageAfter),
    m_timer_s(kTransitionTime),
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

void TransitionScene::Update(float deltaTime_s)
{
    (this->*m_updateFunc)(deltaTime_s);
    m_timer_s -= deltaTime_s;
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

void TransitionScene::UpdateBeforeScene(float deltaTime_s)
{
    if (m_timer_s < 0.0f)
    {
        m_timer_s = kTransitionTime;
        m_updateFunc = &TransitionScene::UpdateAfterScene;
        m_renderFunc = &TransitionScene::RenderAfterScene;
    }
}

void TransitionScene::UpdateAfterScene(float deltaTime_s)
{
    if (m_timer_s < 0.0f)
    {
        m_timer_s = kTransitionTime;
        EnableChangeScene = true;
    }
}

void TransitionScene::UpdateCrossOverScene(float deltaTime_s)
{
    if (m_timer_s < 0.0f)
    {
        m_timer_s = kTransitionTime;
        EnableChangeScene = true;
    }
}

void TransitionScene::RenderBeforeScene()
{
    int alpha = static_cast<int>((m_timer_s * 255) / kTransitionTime);
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
    m_before->Render();
    DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void TransitionScene::RenderAfterScene()
{
    int alpha = static_cast<int>((m_timer_s * 255) / kTransitionTime);
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, 255 - alpha);
    m_after->Render();
    DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void TransitionScene::RenderCrossOverScene()
{
    int alpha = static_cast<int>((m_timer_s * 255) / kTransitionTime);
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
    m_before->Render();
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ADD, 255 - alpha);
    m_after->Render();
    DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
