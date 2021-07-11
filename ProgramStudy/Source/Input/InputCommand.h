#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <chrono>

#include "../Utilities/static_ring.h"

#include "IInput.h"

class InputCommand
{
public:
	struct InputInfo
	{
		INPUT_ID id;
		std::chrono::steady_clock::time_point time;
	};

	using PatternMap_t = std::unordered_map<std::string, std::vector<INPUT_ID>>;

public:
	explicit InputCommand(const std::shared_ptr<IInput>& controller, float duration_s);
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
	static_ring<InputInfo, 10> m_inputs;
	PatternMap_t m_patternMap;
	float m_duration_s;
};

