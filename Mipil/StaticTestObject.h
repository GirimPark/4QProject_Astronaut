#pragma once
#include "../Engine/GameObject.h"

class StaticMeshComponent;
class ParticleComponent;

class StaticTestObject
	: public GameObject
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::weak_ptr<StaticMeshComponent> m_meshComponent;
	std::weak_ptr<ParticleComponent> m_ParticleComponent1;
	std::weak_ptr<ParticleComponent> m_ParticleComponent2;
	std::weak_ptr<ParticleComponent> m_ParticleComponent3;
};

