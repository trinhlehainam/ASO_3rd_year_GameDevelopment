#pragma once
#include "ICollider.h"
#include "../../Math/Geometry/Circle.h"

class CircleCollider : public ICollider
{
public:
	Circle GetCollider() const;
private:

	Circle m_collider;
};

