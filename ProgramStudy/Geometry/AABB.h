#pragma once
#include "IShape.h"
class AABB :
    public IShape
{
public:
    AABB();
    explicit AABB(float x, float y, float width, float height, unsigned int color);
    explicit AABB(vec2f pos, vec2f size, unsigned int color);
    explicit AABB(vec2f pos, vec2f speed, vec2f size, unsigned int color);
    ~AABB();

    bool ConstrainPosition(float width, float height) override;
    void Update(float deltaTime_ms) override;
    void SpecialAction() override;
    void Draw() override;
    void Draw(float scale) override;

    vec2f Size;
};

