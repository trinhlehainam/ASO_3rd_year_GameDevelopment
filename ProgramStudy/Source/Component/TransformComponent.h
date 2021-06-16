#pragma once
#include "IComponent.h"

#include "../Math/vec2.h"

class TransformComponent :
    public IComponent
{
public:
    TransformComponent(const std::shared_ptr<Entity>& owner, const vec2f& pos, const vec2f& size, float scale = 1.0f);
    ~TransformComponent();

public:
    vec2f Pos;
    vec2f Size;
    float Scale;

public:
    void Init() override;
    void Update(float deltaTime_s) override;
    void Render() override;

};

