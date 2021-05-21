#pragma once
#include "IShape.h"

#include <memory>
#include <vector>

class ChristmasTree :
    public IShape
{
public:
    explicit ChristmasTree(vec2f pos, vec2f speed);
    ~ChristmasTree();

    virtual bool ConstrainPosition(float width, float height) override;
    virtual void Update(float deltaTime_ms) override;
    virtual void SpecialAction() override;
    virtual void Draw() override;
    virtual void Draw(float scale) override;

    std::vector<std::unique_ptr<IShape>> Elements;
};

