#include "InputCommand.h"

#include "../_debug/_DebugConOut.h"

InputCommand::InputCommand(const std::shared_ptr<IInput>& controller):m_controller(controller)
{
	m_inputs.set_tail_to_head_loop();
}

InputCommand::~InputCommand()
{
}

bool InputCommand::IsMatch(const std::string& key)
{
	const auto& pattern = m_patternMap.at(key);
	if (pattern.empty()) return false;

	const auto kMatchNum = pattern.size();
	size_t matchIndex = 0;
	for (const auto& input : m_inputs)
	{
		matchIndex = input == pattern[matchIndex] ? matchIndex + 1 : 0;
		if (matchIndex >= kMatchNum)
			return true;
	}
	
	return false;
}

void InputCommand::Update()
{
	if (m_controller.expired()) return;

	auto pressedKeys = m_controller.lock()->GetJustPressedKeys();
	if (pressedKeys.empty()) return;

	for (auto key : pressedKeys)
		m_inputs.insert(key);

	TRACE("Inputs Stack : ");
	for (const auto& input : m_inputs)
	{
		TRACE("%d  ", static_cast<int>(input));
	}	
	TRACE("\n");

	for (const auto& pattern : m_patternMap)
	{
		TRACE("Pattern %s :", pattern.first.c_str());
		for (const auto& key : pattern.second)
			TRACE("%d  ", static_cast<int>(key));
	}
	TRACE("\n");
}
