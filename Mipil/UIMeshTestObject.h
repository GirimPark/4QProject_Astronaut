#pragma once
#include "../Engine/GameObject.h"

class UIMeshComponent;
class SkeletalMeshComponent;
class MaterialManagerComponent;
class ParticleComponent;
class StaticMeshComponent;

using namespace std;

class UIMeshTestObject :
    public GameObject
{
public:
    void Initialize() override;
    void Update(float deltaTime) override;

public:
    void ChangedMaterial();
    void ChangedUIMaterial();
    void PlayParticle();
    void ChangedMeshOutLine();

private:
    std::shared_ptr<UIMeshComponent> m_PlayerOneMeshComponent;
    std::shared_ptr<UIMeshComponent> m_PlayerTwoMeshComponent;
    std::shared_ptr<SkeletalMeshComponent> m_SkeletalTestComponent;
    std::shared_ptr<MaterialManagerComponent> m_MaterialManagerComponent;

    // 충격 파티클
    std::shared_ptr<ParticleComponent> m_ShockParticleComponentOne;
    std::shared_ptr<ParticleComponent> m_ShockParticleComponentTwo;
    std::shared_ptr<ParticleComponent> m_ShockParticleComponentThree;
    std::shared_ptr<ParticleComponent> m_ShockParticleComponentFour;

    // 불꽃 파티클
    std::shared_ptr<ParticleComponent> m_FireParticleComponentOne;
    std::shared_ptr<ParticleComponent> m_FireParticleComponentTwo;
    std::shared_ptr<ParticleComponent> m_FireParticleComponentThree;

    // 연기 파티클
    std::shared_ptr<ParticleComponent> m_SmokeParticleComponentOne;
    std::shared_ptr<ParticleComponent> m_SmokeParticleComponentTwo;
    std::shared_ptr<ParticleComponent> m_SmokeParticleComponentThree;
    std::shared_ptr<ParticleComponent> m_SmokeParticleComponentFour;

    // 반짝임 파티클
    std::shared_ptr<ParticleComponent> m_SparklesParticleComponentOne;
    std::shared_ptr<ParticleComponent> m_SparklesParticleComponentTwo;
    std::shared_ptr<ParticleComponent> m_SparklesParticleComponentThree;
};

