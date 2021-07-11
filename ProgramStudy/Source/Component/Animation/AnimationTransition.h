#pragma once
#include <vector>

#include "CompareMode.h"
#include "AnimatorCondition.h"

struct AnimatorTransition
{
	std::vector<AnimatorCondition> conditions;
	std::string currentState;
	std::string destinationState;
	std::string tag;
	bool isMute;

	void AddCondition(COMPARE_MODE mode, const std::string& paraName, float threshold);
};

