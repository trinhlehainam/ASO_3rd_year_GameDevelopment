#pragma once
#include "../Math/vec2.h"

class IShape
{
public:
	enum class TYPE
	{
		AABB,
		CIRCLE,
		TRIANGLE,
		CHRISTMAS_TREE,
	};
public:
	IShape();
	explicit IShape(TYPE type, unsigned int color);
	explicit IShape(TYPE type, float x, float y, unsigned int color);
	explicit IShape(TYPE type, vec2f pos, unsigned int color);
	explicit IShape(TYPE type, vec2f pos, vec2f speed, unsigned int color);
	virtual ~IShape();

public:
	vec2f Pos;
	vec2f Speed;
	TYPE Type;
	bool IsAlive;
	unsigned int Color;

public:
	void SetAlive(bool isAlive);
	void SetColor(unsigned int color);

public:
	// Limit shape's position in Range(width, height)
	virtual bool ConstrainPosition(float width, float height) = 0;
	virtual void Update(float deltaTime_ms) = 0;
	virtual void SpecialAction() = 0;

	virtual void Draw() = 0;
	virtual void Draw(float scale) = 0;
};

