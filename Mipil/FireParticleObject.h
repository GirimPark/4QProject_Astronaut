#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/EventListener.h"

class ParticleComponent;
class Event;

class FireParticleObject
	: public GameObject
	, public EventListener
{
public:
	FireParticleObject();
	~FireParticleObject();

	void PlayParticle(bool isPlaying);

	void SetLocalPositions(Math::Vector3 pos);

	void Initialize() override;
	void Update(float deltaTime) override;

	void HandleEvent(Event* event) override;

private:
	std::shared_ptr<ParticleComponent> m_FireParticleComponentOne;
	std::shared_ptr<ParticleComponent> m_FireParticleComponentTwo;
	std::shared_ptr<ParticleComponent> m_FireParticleComponentThree;

	bool m_bPlay = false;
};
