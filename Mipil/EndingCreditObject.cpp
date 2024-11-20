#include "pch.h"
#include "EndingCreditObject.h"

#include "../Engine/ParticleComponent.h"
#include "../Engine/StaticMeshComponent.h"

void EndingCreditObject::Initialize()
{
	m_pMeshComponent = CreateComponent<StaticMeshComponent>("Rocket").lock();
	m_pMeshComponent->SetFilePath("../Resources/FBX/rokek_basic_ingame.fbx");
	m_pMeshComponent->SetLocalRotation(Vector3(90.f, 0.f, 90.f));
	SetRootComponent(m_pMeshComponent);


	/// 화염 파티클 세팅 영역 ///
	{
		m_FireParticleComponentOne = CreateComponent<ParticleComponent>("FireParticle A").lock();
		m_FireParticleComponentTwo = CreateComponent<ParticleComponent>("FireParticle B").lock();
		m_FireParticleComponentThree = CreateComponent<ParticleComponent>("FireParticle C").lock();
		ParticleComponentInfo info;
		info.ParticleDirection = 0;
		info.bIsLoop = true;
		info.Acceleration = Vector3(10.f, 0.f, 0.f);
		info.CreatePerSecond = 5.f;
		info.CreateTime = 5.f;
		info.GenerationRange = Vector3(10.f, 15.f, 15.f);
		info.InitialSpeed = Vector3(0.f, 3.f, 3.f);
		info.LifeTime = 2.0f;
		info.m_ParticleType = eParticleType::FIRE;
		info.m_ShaderPath = L"../Shader/Fire.hlsl";
		info.Size = Vector2(100.f, 100.f);
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_1.png";
		m_FireParticleComponentOne->Setting(info);
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_2.png";
		info.CreatePerSecond = 20.f;
		m_FireParticleComponentTwo->Setting(info);
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_3.png";
		info.CreatePerSecond = 20.f;
		m_FireParticleComponentThree->Setting(info);

		m_FireParticleComponentOne->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));
		m_FireParticleComponentTwo->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));
		m_FireParticleComponentThree->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));
	}

	GameObject::Initialize();
}

void EndingCreditObject::Update(float deltaTime)
{
	Vector3 rocketMovePosition = m_pMeshComponent->GetLocalPosition() + deltaTime * Vector3(-100.f, 0.f, 0.f);
	m_pMeshComponent->SetLocalPosition(rocketMovePosition);

	m_FireParticleComponentOne->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));
	m_FireParticleComponentTwo->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));
	m_FireParticleComponentThree->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));

	GameObject::Update(deltaTime);
}
