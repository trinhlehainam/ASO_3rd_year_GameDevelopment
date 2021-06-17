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
    bool LoadAnimationFromXML(const std::string& file);

public:
    void Init() override;
    void Update(float deltaTime_s) override;
    void Render() override;

private:
    std::weak_ptr<TransformComponent> m_transform;
    int m_textureId;
};

