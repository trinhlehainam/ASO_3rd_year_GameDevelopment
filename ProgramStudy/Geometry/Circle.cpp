#include "Circle.h"

#include "../Math/MathHelper.h"

#include <DxLib.h>

Circle::Circle():IShape(TYPE::CIRCLE, GetColor(0,255,0)),Radius(0.0f)
{
}

Circle::Circle(float x, float y, float radius, unsigned int color):IShape(TYPE::CIRCLE, x,y,color), Radius(radius)
{
}

Circle::Circle(vec2f pos, float radius, unsigned int color):IShape(TYPE::CIRCLE, pos,color),Radius(radius)
{
}

Circle::Circle(vec2f pos, vec2f speed, float radius, unsigned int color):IShape(TYPE::CIRCLE, pos,speed,color),Radius(radius)
{
}

void Circle::Draw()
{
	DxLib::DrawCircleAA(Pos.x, Pos.y, Radius, 32, Color);
}

void Circle::Draw(float scale)
{
	DxLib::DrawCircleAA(Pos.x, Pos.y, Radius * scale, 32, Color);
}

bool Circle::ConstrainPosition(float width, float height)
{
	vec2f n;
	if (Pos.x - Radius <= 0)
	{
		n = vec2f(1.0f, 0.0f);
		Pos.x = Radius;
	}
	else if (Pos.x + Radius >= width)
	{
		n = vec2f(-1.0f, 0.0f);
		Pos.x = width - Radius;
	}
	else if (Pos.y - Radius <= 0)
	{
		n = vec2f(0.0f, 1.0f);
		Pos.y = Radius;
	}
	else if (Pos.y + Radius >= height)
	{
		n = vec2f(0.0f, -1.0f);
		Pos.y = height - Radius;
	}
	if (n == 0.0f) return false;

	Speed = reflectionVec(Speed, n);

	return true;
}

void Circle::Update(float deltaTime_ms)
{
	Pos += Speed * deltaTime_ms;
}

void Circle::SpecialAction()
{
	SetAlive(false);
}
