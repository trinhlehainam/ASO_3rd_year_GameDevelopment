#include "Input.h"

#include <DxLib.h>

namespace
{
	constexpr int kMaxKeyNum = 256;
	std::array<char, kMaxKeyNum> gKeyStates;
}

Input::Input():
	m_currentState(0),
	m_inputs{
	KEY_INPUT_UP,
	KEY_INPUT_DOWN,
	KEY_INPUT_LEFT,
	KEY_INPUT_RIGHT,
	KEY_INPUT_Z,
	KEY_INPUT_X,
	KEY_INPUT_C,
	KEY_INPUT_P
	}
{
}

Input::~Input()
{
}

void Input::Update()
{
	DxLib::GetHitKeyStateAll(gKeyStates.data());

	m_currentState = (m_currentState + 1) % kMaxInputStates;

	for (auto id : INPUT_ID())
		m_inputStates[m_currentState].set(static_cast<size_t>(id), gKeyStates[m_inputs[static_cast<size_t>(id)]]);
}

bool Input::IsPressed(INPUT_ID btn) const
{
	return m_inputStates[m_currentState][static_cast<size_t>(btn)] == 1;
}

bool Input::IsJustPressed(INPUT_ID btn) const
{
	return m_inputStates[m_currentState][static_cast<size_t>(btn)] == 1 &&
		m_inputStates[PreviousState()][static_cast<size_t>(btn)] == 0;
}

bool Input::IsReleased(INPUT_ID btn) const
{
	return m_inputStates[m_currentState][static_cast<size_t>(btn)] == 0;
}

bool Input::IsJustReleased(INPUT_ID btn) const
{
	return m_inputStates[m_currentState][static_cast<size_t>(btn)] == 0 && 
		m_inputStates[PreviousState()][static_cast<size_t>(btn)] == 1;
}

int Input::PreviousState() const
{
	return ((m_currentState - 1 ) + kMaxInputStates) % kMaxInputStates;
}
