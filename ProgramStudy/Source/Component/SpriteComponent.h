#pragma once
#include "IComponent.h"

#include <string>
#include <unordered_map>
#include <vector>

class TransformComponent;

struct Animation
{
    int texId;
    int texColumns;

    int celWidth, celHeight;
    int celBaseId;
    int celCount;
    int loop;
};

class SpriteComponent :
    public IComponent
{
public:
    explicit SpriteComponent(const std::shared_ptr<Entity>& owner);
    ~SpriteComponent();

public:
    bool LoadAnimationFromXML(const std::string& file, const std::string& animationKey);
    bool Play(const std::string& animKey, const std::string& state);

public:
    void Init() override;
    void Update(float deltaTime_s) override;
    void Render() override;

private:
    std::weak_ptr<TransformComponent> m_transform;
    std::unordered_map<std::string, Animation> m_animations;
    std::vector<int> m_durations;

    std::string m_currentAnimKey;
    int m_currentDurationId;
    int m_timer_ms;
};

