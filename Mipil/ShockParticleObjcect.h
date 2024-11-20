#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/EventListener.h"

class ParticleComponent;
class Event;

class ShockParticleObjcect
	: public GameObject
	, public EventListener

{
public:
	ShockParticleObjcect();
	~ShockParticleObjcect();

	void PlayParticle();

	void Initialize() override;
	void Update(float deltaTime) override;

	// EventListener을(를) 통해 상속됨
	void HandleEvent(Event* event) override;

private:
	// 충격 파티클
	std::shared_ptr<ParticleComponent> m_ShockParticleComponentOne;
	std::shared_ptr<ParticleComponent> m_ShockParticleComponentTwo;
	std::shared_ptr<ParticleComponent> m_ShockParticleComponentThree;
	std::shared_ptr<ParticleComponent> m_ShockParticleComponentFour;
};

