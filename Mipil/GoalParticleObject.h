#pragma once
#include "../Engine/GameObject.h"

class ParticleComponent;

class GoalParticleObject :
    public GameObject
{
public:
    GoalParticleObject();
	~GoalParticleObject();

    void SetGoalPosition(int x, int y, int z);

	void Initialize() override;
	void Update(float deltaTime) override;

private:
    // 반짝임 파티클
    std::shared_ptr<ParticleComponent> m_SparklesParticleComponentOne;
    std::shared_ptr<ParticleComponent> m_SparklesParticleComponentTwo;
    std::shared_ptr<ParticleComponent> m_SparklesParticleComponentThree;
};

