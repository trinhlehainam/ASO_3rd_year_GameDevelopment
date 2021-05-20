#pragma once
#include "IShape.h"

class Triangle :
    public IShape
{
public:
	explicit Triangle(vec2f a, vec2f b, vec2f c);
	explicit Triangle(vec2f a, vec2f b, vec2f c, vec2f speed);

	void ConstrainPosition(float width, float height) override;
	void Update(float deltaTime_ms) override;
	void Draw() override;
	void Draw(float scale) override;
	void SpecialAction() override;

	vec2f Points[3];
};

