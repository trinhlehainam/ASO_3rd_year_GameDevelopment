#include "AABB.h"

#include <iostream>

#include <DxLib.h>
#include "../Math/MathHelper.h"

AABB::AABB():IShape(TYPE::AABB, GetColor(255,0,0)),Size()
{
}

AABB::AABB(float x, float y, float width, float height, unsigned int color):IShape(TYPE::AABB, x,y, color),Size(width,height)
{
}

AABB::AABB(vec2f pos, vec2f size, unsigned int color) : IShape(TYPE::AABB, pos, color), Size(size)
{
}

AABB::AABB(vec2f pos, vec2f speed, vec2f size, unsigned int color):IShape(TYPE::AABB, pos,speed, color),Size(size)
{
}

AABB::~AABB()
{
}

void AABB::Draw()
{
	DxLib::DrawBoxAA(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y, Color, 1);
}

void AABB::Draw(float scale)
{
	DxLib::DrawBoxAA(Pos.x, Pos.y, (Pos.x + Size.x) * scale, (Pos.y + Size.y) * scale, Color, 1);
}

bool AABB::ConstrainPosition(float width, float height)
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

	if (n == 0.0f) return false;
	Speed = reflectionVec(Speed, n);

	return true;
}

void AABB::Update(float deltaTime_ms)
{
	Pos += Speed * deltaTime_ms;
}

void AABB::SpecialAction()
{
	SetAlive(false);
}
