#pragma once
#include "../IComponent.h"

#include <string>

class Animator : public IComponent
{
public:
	explicit Animator(const std::shared_ptr<Entity>& owner);
	~Animator();

	void AddAnimatorController(const std::string& path);

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
	class Impl;
	std::unique_ptr<Impl> m_impl;
};

