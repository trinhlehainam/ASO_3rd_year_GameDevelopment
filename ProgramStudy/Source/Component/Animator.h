#pragma once
#include "IComponent.h"

#include <string>
#include <vector>
#include <unordered_map>

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

	// Animation Map : <StateKey, key to access Animation in AnimationMng>
	std::unordered_map<std::string, std::string> m_animationMap;

	// TODO : Implement Transition


	// Parameters
	std::unordered_map<std::string, float> m_floatParams;
	std::unordered_map<std::string, bool> m_boolParams;
	std::unordered_map<std::string, int> m_intParams;

	// TODO : Implement parameter ID
	std::string m_state;
	int m_currentDurationId;
	int m_timer_ms;
	int m_loopCount;
};

