#pragma once
#include "IShape.h"
class Circle :
    public IShape
{
public:
    Circle();
    explicit Circle(float x, float y, float radius, unsigned int color);
    explicit Circle(vec2f pos, float radius, unsigned int color);
    explicit Circle(vec2f pos, vec2f speed, float radius, unsigned int color);

    bool ConstrainPosition(float width, float height) override;
    void Update(float deltaTime_ms) override;
    void Draw() override;
    void Draw(float scale) override;
    void SpecialAction() override;

    float Radius;
};

