#include "AABB.h"

#include <iostream>

#include <DxLib.h>
#include "../Math/MathHelper.h"

AABB::AABB():IShape(TYPE::AABB),Size()
{
}

AABB::AABB(float x, float y, float width, float height):IShape(TYPE::AABB, x,y),Size(width,height)
{
}

AABB::AABB(vec2f pos, vec2f size) : IShape(TYPE::AABB, pos), Size(size)
{
}

AABB::AABB(vec2f pos, vec2f speed, vec2f size):IShape(TYPE::AABB, pos,speed),Size(size)
{
}

AABB::~AABB()
{
}

void AABB::Draw()
{
	DxLib::DrawBoxAA(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y, GetColor(255, 0, 0), 1);
}

void AABB::Draw(float scale)
{
	DxLib::DrawBoxAA(Pos.x, Pos.y, (Pos.x + Size.x) * scale, (Pos.y + Size.y) * scale, GetColor(255, 0, 0), 1);
}

void AABB::ConstrainPosition(float width, float height)
{
	vec2f n;
	if (Pos.x <= 0.0f)
	{
		n = vec2f(1.0f, 0.0f);
		Pos.x = 0.0f;
	}
	else if (Pos.x + Size.x >= width)
	{
		n = vec2f(-1.0f, 0.0f);
		Pos.x = width - Size.x;
	}
	else if (Pos.y <= 0.0f)
	{
		n = vec2f(0.0f, 1.0f);
		Pos.y = 0.0f;
	}
	else if (Pos.y + Size.y >= height)
	{
		n = vec2f(0.0f, -1.0f);
		Pos.y = height - Size.y;
	}
	Speed = reflectionVec(Speed, n);
}

void AABB::Update(float deltaTime_ms)
{
	Pos += Speed * deltaTime_ms;
}

void AABB::SpecialAction()
{
	Speed = -Speed;
}
