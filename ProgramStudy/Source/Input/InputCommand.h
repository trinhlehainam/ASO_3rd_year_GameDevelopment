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

public:
	explicit InputCommand(const std::shared_ptr<IInput>& controller);
	~InputCommand();

	template<typename...Args>
	void AddPattern(std::string key, Args&&...args)
	{
		assert(!m_patternMap.count(key));
		(m_patternMap[key].push_back(std::forward<Args>(args)),...);
	}

	bool IsMatch(const std::string& patternKey);

	void Update();
private:
	std::weak_ptr<IInput> m_controller;
	static_ring<INPUT_ID, 10> m_inputs;
	PatternMap_t m_patternMap;
};

