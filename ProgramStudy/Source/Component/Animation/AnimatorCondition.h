#pragma once
#include <string>

#include "CompareMode.h"

struct AnimatorCondition
{
	COMPARE_MODE compareMode;
	std::string paramName;
	float threshold;
};