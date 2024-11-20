#include "pch.h"
#include "CreditObject.h"

#include "../Engine/ParticleComponent.h"
#include "../Engine/StaticMeshComponent.h"
#include"../Engine/SkeletalMeshComponent.h"
#include "../D3DRenderer/StaticMeshModel.h"
#include "../D3DRenderer/StaticMeshInstance.h"

void CreditObject::Initialize()
{
	/// ·ÎÄÏ ///
	m_pMeshComponent = CreateComponent<StaticMeshComponent>("Rocket").lock();
	m_pMeshComponent->SetFilePath("../Resources/FBX/rokek_basic_ingame.fbx");
	m_pMeshComponent->SetLocalRotation(Vector3(90.f, 0.f, 90.f));

	SetRootComponent(m_pMeshComponent);

	/// °õÇÏ°í Åä³¢ ¼Õ Èçµå´Â ¸ðµ¨ ///
	m_pBearMeshComponent = CreateComponent<SkeletalMeshComponent>("Bear").lock();
	m_pBearMeshComponent->SetDefaultAnimation("../Resources/FBX/bear_with_armature.fbx");
	m_pRabbitMeshComponent = CreateComponent<SkeletalMeshComponent>("Rabbit").lock();
	m_pRabbitMeshComponent->SetDefaultAnimation("../Resources/FBX/rabbit_basic.fbx");

	m_pRabbitMeshComponent->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	m_pBearMeshComponent->SetLocalScale(Vector3(0.5f, 0.5f, 0.5f));
	m_pRabbitMeshComponent->SetLocalRotation(Vector3(5.f, 0.f, 45.f));
	m_pBearMeshComponent->SetLocalRotation(Vector3(0.f, 0.f, -45.f));
	m_pRabbitMeshComponent->SetLocalPosition(Vector3(m_pMeshComponent->GetLocalPosition()));
	m_pBearMeshComponent->SetLocalPosition(Vector3(m_pMeshComponent->GetLocalPosition() + Vector3(50.f, 0.f, 0.f)));


	/// È­¿° ÆÄÆ¼Å¬ ¼¼ÆÃ ¿µ¿ª ///
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

	for (auto& instance : m_pMeshComponent->GetStaticMeshModel()->m_MeshInstances)
	{
		instance.m_pBoundingBox->Extents = Vector3(500.f, 500.f, 500.f);
	}
}

void CreditObject::Update(float deltaTime)
{
	Vector3 rocketMovePosition = m_pMeshComponent->GetLocalPosition() + deltaTime * Vector3(-120.f, 0.f, 0.f);
	m_pMeshComponent->SetLocalPosition(rocketMovePosition);

	m_FireParticleComponentOne->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));
	m_FireParticleComponentTwo->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));
	m_FireParticleComponentThree->SetLocalPosition(m_pMeshComponent->GetLocalPosition() + Vector3(-60.f, 0.f, 0.f));
	m_pRabbitMeshComponent->SetLocalPosition(Vector3(m_pMeshComponent->GetLocalPosition()) + Vector3(-420.f, -40.f, -80.f));
	m_pBearMeshComponent->SetLocalPosition(Vector3(m_pMeshComponent->GetLocalPosition() + Vector3(-600.f, -85.f, -95.f)));

	GameObject::Update(deltaTime);
}
