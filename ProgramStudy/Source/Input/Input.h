#pragma once
#include <bitset>
#include <array>

#include "InputID.h"

namespace
{
	constexpr int kMaxInputStates = 2;
}

class Input
{
public:
	Input();
	~Input();
public:
	void Update();

public:
	bool IsPressed(INPUT_ID btn) const;
	bool IsJustPressed(INPUT_ID btn) const;
	bool IsReleased(INPUT_ID btn) const;
	bool IsJustReleased(INPUT_ID btn) const;
private:

	int PreviousState() const;
private:
	using ArrayInputs_t = std::array<int, static_cast<size_t>(INPUT_ID::MAX)>;
	using InputState_t = std::bitset<static_cast<size_t>(INPUT_ID::MAX)>;

	ArrayInputs_t m_inputs;
	std::array<InputState_t, kMaxInputStates> m_inputStates;
	int m_currentState;
};

