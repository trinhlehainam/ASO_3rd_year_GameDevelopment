#pragma once
#include "IComponent.h"

#include <string>
#include <unordered_map>

class TransformComponent;

struct Animation
{
    // Source Info
    int texId;
    int celWidth, celHeight;
    int texWidth, texHeight;

    // Animation Info
    unsigned int celId;
    unsigned int celCount;
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

public:
    void Init() override;
    void Update(float deltaTime_s) override;
    void Render() override;

private:
    std::weak_ptr<TransformComponent> m_transform;
    std::unordered_map<std::string, Animation> m_animations;
    std::string m_currentAnim;
};

