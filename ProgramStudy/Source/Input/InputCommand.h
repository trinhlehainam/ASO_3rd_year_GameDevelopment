#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "../Utilities/static_ring.h"

#include "IInput.h"

class InputCommand
{
public:
	struct InputInfo
	{
		INPUT_ID id;
		float time;
	};

	using PatternMap_t = std::unordered_map<std::string, std::vector<INPUT_ID>>;
private:
	std::weak_ptr<IInput> m_bind;
	static_ring<INPUT_ID, 10> m_inputs;
	PatternMap_t m_patternMap;
};

