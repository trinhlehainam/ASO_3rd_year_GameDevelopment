#pragma once
#include "IComponent.h"

#include <string>

class TransformComponent;

class SpriteComponent :
    public IComponent
{
public:
    explicit SpriteComponent(const std::shared_ptr<Entity>& owner);
    ~SpriteComponent();

public:
    bool Play(const std::string& animKey, const std::string& state);
    bool IsPlaying(const std::string& animKey, const std::string& state);

public:
    void Init() override;
    void Update(float deltaTime_s) override;
    void Render() override;

private:
    void UpdateInfinite(float deltaTime_s);
    void UpdateLoop(float deltaTime_s);
    void UpdateSleep(float deltaTime_s);

private:
    using UpdateFunc_t = void (SpriteComponent::*)(float);
    UpdateFunc_t m_updateFunc;

private:
    std::weak_ptr<TransformComponent> m_transform;

    std::string m_animKey;
    int m_currentDurationId;
    int m_timer_ms;
    int m_loopCount;
};

