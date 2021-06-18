#pragma once
#include "IComponent.h"

#include <string>
#include <unordered_map>

#include "../Math/rect.h"

class TransformComponent;

struct Animation
{
    // Source Info
    int texId;
    rect srcRect;
    vec2f texSize;

    // Animation Info
    unsigned int index;
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
    int m_textureId;
};

