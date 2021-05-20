#include "Triangle.h"

#include <DxLib.h>

Triangle::Triangle(vec2f a, vec2f b, vec2f c) :Points{ a,b,c }, IShape(IShape::TYPE::TRIANGLE)
{
}

Triangle::Triangle(vec2f a, vec2f b, vec2f c, vec2f speed) : Points{ a,b,c }, IShape(IShape::TYPE::TRIANGLE)
{
	Speed = speed;
}

void Triangle::Draw()
{
	DxLib::DrawTriangleAA(Points[0].x, Points[0].y, Points[1].x, Points[1].y, Points[2].x, Points[2].y, GetColor(0, 0, 255), 1);
}

void Triangle::Draw(float scale)
{
	DxLib::DrawTriangleAA(Points[0].x, Points[0].y, Points[1].x, Points[1].y, Points[2].x, Points[2].y, GetColor(0, 0, 255), 1);
}

void Triangle::ConstrainPosition(float width, float height)
{
}

void Triangle::Update(float deltaTime_ms)
{
	for (auto& point : Points)
		point += Speed * deltaTime_ms;
}

void Triangle::SpecialAction()
{
}
