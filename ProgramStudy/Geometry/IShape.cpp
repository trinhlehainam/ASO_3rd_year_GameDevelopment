#include "IShape.h"

IShape::IShape():Pos(),IsAlive(true)
{
}

IShape::IShape(TYPE type):Type(type), IsAlive(true)
{
}

IShape::IShape(TYPE type, float x, float y):Pos(x,y),Type(type), IsAlive(true)
{
}

IShape::IShape(TYPE type, vec2f pos):Pos(pos), Type(type), IsAlive(true)
{
}

IShape::IShape(TYPE type, vec2f pos, vec2f speed):Pos(pos),Speed(speed), Type(type), IsAlive(true)
{
}

IShape::~IShape()
{
}

void IShape::SetAlive(bool isAlive)
{
	IsAlive = isAlive;
}

void IShape::SetColor(unsigned int color)
{
	Color = color;
}
