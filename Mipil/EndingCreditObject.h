#pragma once
#include "../Engine/GameObject.h"

class StaticMeshComponent;
class ParticleComponent;

using namespace std;

class EndingCreditObject : public GameObject
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::shared_ptr<StaticMeshComponent> m_pMeshComponent;

	// ºÒ²É ÆÄÆ¼Å¬
	std::shared_ptr<ParticleComponent> m_FireParticleComponentOne;
	std::shared_ptr<ParticleComponent> m_FireParticleComponentTwo;
	std::shared_ptr<ParticleComponent> m_FireParticleComponentThree;
};

