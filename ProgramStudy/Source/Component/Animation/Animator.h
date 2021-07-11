#pragma once
#include "../IComponent.h"

#include <string>
#include <vector>
#include <unordered_map>

#include "AnimatorState.h"

class TransformComponent;

class Animator : public IComponent
{
public:
	enum class PARAMETER_TYPE
	{
		FLOAT,
		BOOL,
		INTEGER
	};
public:
	explicit Animator(const std::shared_ptr<Entity>& owner);
	~Animator();

	void AddParameter(const std::string& name, PARAMETER_TYPE paramType);

	void SetFloat(const std::string& name, float value);
	void SetBool(const std::string& name, bool flag);
	void SetInteger(const std::string& name, int value);

	float GetFloat(const std::string& name);
	bool GetBool(const std::string& name);
	int GetInteger(const std::string& name);

	void Play(const std::string& state);
public:
	void Init() override;
	void Update(float deltaTime_s) override;
	void Render() override;

private:
	bool HasParameter(const std::string& name);
private:
	std::weak_ptr<TransformComponent> m_tranform;
	
	std::unordered_map<std::string, AnimatorState> m_stateMap;

	// Parameters
	std::unordered_map<std::string, float> m_floatParams;
	std::unordered_map<std::string, bool> m_boolParams;
	std::unordered_map<std::string, int> m_intParams;

	std::string m_currentAnimState;
	int m_currentDurationId;
	int m_timer_ms;
	int m_loopCount;
};

