#pragma once
#include "IScene.h"

class TransitionScene :
    public IScene
{
public:
    enum class STAGE
    {
        FADE_IN,
        FADE_OUT,
        CROSS_OVER
    };
public:
    TransitionScene(std::unique_ptr<IScene> before, std::unique_ptr<IScene> after);
    TransitionScene(std::unique_ptr<IScene> before, std::unique_ptr<IScene> after, STAGE stage);
    TransitionScene(std::unique_ptr<IScene> before, std::unique_ptr<IScene> after, STAGE stageBefore, STAGE stageAfter );
    ~TransitionScene();
private:
   bool Init() override;
   void Update(float deltaTime_s) override;
   void Render() override;
   std::unique_ptr<IScene> ChangeScene(std::unique_ptr<IScene> scene) override;

private:
   using UpdateFunc_t = void (TransitionScene::*)(float);
   using RenderFunc_t = void (TransitionScene::*)();
   
   void UpdateBeforeScene(float deltaTime_s);
   void UpdateAfterScene(float deltaTime_s);
   void UpdateCrossOverScene(float deltaTime_s);

   void RenderBeforeScene();
   void RenderAfterScene();
   void RenderCrossOverScene();
private:
    std::unique_ptr<IScene> m_before;
    std::unique_ptr<IScene> m_after;
    STAGE m_stageBefore;
    STAGE m_stageAfter;

    RenderFunc_t m_renderFunc;
    UpdateFunc_t m_updateFunc;

    float m_timer_s;
};

