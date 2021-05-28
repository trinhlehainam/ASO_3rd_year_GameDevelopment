#pragma once
#include "IScene.h"

class TransitionScene :
    public IScene
{
public:
    enum class LAYOUT
    {
        FADE_IN,
        FADE_OUT
    };
public:
    TransitionScene(std::unique_ptr<IScene> before, std::unique_ptr<IScene> after);
    ~TransitionScene();
private:
   bool Init() override;
   void Update(float deltaTime_s) override;
   void Render() override;
   std::unique_ptr<IScene> ChangeScene(std::unique_ptr<IScene> scene) override;

private:
   using UpdateFunc_t = void (TransitionScene::*)(float);
   using RenderFunc_t = void (TransitionScene::*)();
   
   void UpdateBeforeScene(float deltaTime);
   void UpdateAfterScene(float deltaTime);
   void RenderBeforeScene();
   void RenderAfterScene();
private:
    std::unique_ptr<IScene> m_before;
    std::unique_ptr<IScene> m_after;

    RenderFunc_t m_renderFunc;
    UpdateFunc_t m_updateFunc;

    float m_timer_s;
};

