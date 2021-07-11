#pragma once
#include "AnimationTransition.h"

struct AnimatorState
{
	std::string motionName;
	std::vector<AnimatorTransition> transitions;
	std::string tag;
};
