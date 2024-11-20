#include "pch.h"
#include "UIMeshTestObject.h"

#include "../Engine/UIMeshComponent.h"
#include "../Engine/SkeletalMeshComponent.h"
#include "../Engine/MaterialManagerComponent.h"
#include "../Engine/ParticleComponent.h"

/// 3DUIMeshComponent, MaterialManagerComponent, ParticleComponent 세팅 에제 코드 ///
void UIMeshTestObject::Initialize()
{
	m_SkeletalTestComponent = CreateComponent<SkeletalMeshComponent>("MaterialTestComponent").lock();
	m_SkeletalTestComponent->SetDefaultAnimation("../Resources/FBX/bear_with_armature.fbx");
	m_SkeletalTestComponent->SetIsOutLine(true);
	SetRootComponent(m_SkeletalTestComponent);

	/// Player Mesh UI 컴포넌트 세팅 영역 ///
	{
		m_PlayerOneMeshComponent = CreateComponent<UIMeshComponent>("UIMeshComponent").lock();
		m_PlayerTwoMeshComponent = CreateComponent<UIMeshComponent>("UIMeshComponent").lock();
		m_MaterialManagerComponent = CreateComponent<MaterialManagerComponent>("MaterialManagerTestComponent").lock();
		m_PlayerOneMeshComponent->SetDefaultAnimation("../Resources/FBX/bear_with_armature.fbx");
		m_PlayerTwoMeshComponent->SetDefaultAnimation("../Resources/FBX/bear_with_armature.fbx");
		m_PlayerOneMeshComponent->SetPlayerNumber(1);
		m_PlayerTwoMeshComponent->SetPlayerNumber(2);
	}

	/// 머터리얼 관리자 컴포넌트 세팅 영역 ///
	{
		m_MaterialManagerComponent->MaterialSetting("TestMaterial", L"../Resources/Textures/1_Base_color.png", MaterialMapFlags::BASECOLOR);
		m_MaterialManagerComponent->MaterialSetting("TestMaterial", L"../Resources/Textures/1_Metallic.png", MaterialMapFlags::METALNESS);
		m_MaterialManagerComponent->MaterialSetting("TestMaterial", L"../Resources/Textures/1_Normal_OpenGL.png", MaterialMapFlags::NORMAL);
		m_MaterialManagerComponent->MaterialSetting("TestMaterial", L"../Resources/Textures/1_Roughness.png", MaterialMapFlags::ROUGHNESS);
	}

	/// 충격 파티클 세팅 영역 ///
	{
		m_ShockParticleComponentOne = CreateComponent<ParticleComponent>("ShockParticleComponentOne").lock();
		m_ShockParticleComponentTwo = CreateComponent<ParticleComponent>("ShockParticleComponentTwo").lock();
		m_ShockParticleComponentThree = CreateComponent<ParticleComponent>("ShockParticleComponentThree").lock();
		m_ShockParticleComponentFour = CreateComponent<ParticleComponent>("ShockParticleComponentFour").lock();
		ParticleComponentInfo info;
		info.bIsLoop = false;
		info.Acceleration = Vector3(0.f, -150.f, 0.f);
		info.CreatePerSecond = 50.f;
		info.CreateTime = 0.2f;
		info.GenerationRange = Vector3(0.f, 0.f, 0.f);
		info.InitialSpeed = Vector3(100.f, 100.f, 100.f);
		info.LifeTime = 1.f;
		info.m_ParticleType = eParticleType::SHOCK_STAR;
		info.m_ShaderPath = L"../Shader/ShockStar.hlsl";
		info.Size = Vector2(15.f, 15.f);
		info.m_TexturePath = L"../Resources/Textures/Particle_star_1.png";
		m_ShockParticleComponentOne->Setting(info);
		info.m_TexturePath = L"../Resources/Textures/Particle_star_2.png";
		m_ShockParticleComponentTwo->Setting(info);
		info.CreatePerSecond = 50.f;
		info.Acceleration = Vector3(0.f, -200.f, 0.f);
		info.InitialSpeed = Vector3(150.f, 150.f, 150.f);
		info.LifeTime = 0.6f;
		info.Size = Vector2(8.f, 8.f);
		info.m_TexturePath = L"../Resources/Textures/Particle_star_1.png";
		m_ShockParticleComponentThree->Setting(info);
		info.Size = Vector2(5.f, 5.f);
		info.m_TexturePath = L"../Resources/Textures/Particle_star_2.png";
		m_ShockParticleComponentFour->Setting(info);

		m_ShockParticleComponentOne->SetLocalPosition(Vector3(0.f, 0.f, -200.f));
		m_ShockParticleComponentTwo->SetLocalPosition(Vector3(0.f, 0.f, -200.f));
		m_ShockParticleComponentThree->SetLocalPosition(Vector3(0.f, 0.f, -200.f));
		m_ShockParticleComponentFour->SetLocalPosition(Vector3(0.f, 0.f, -200.f));
	}

	/// 화염 파티클 세팅 영역 ///
	{
		m_FireParticleComponentOne = CreateComponent<ParticleComponent>("FireParticleComponentOne").lock();
		m_FireParticleComponentTwo = CreateComponent<ParticleComponent>("FireParticleComponentTwo").lock();
		m_FireParticleComponentThree = CreateComponent<ParticleComponent>("FireParticleComponentThree").lock();
		ParticleComponentInfo info;
		info.ParticleDirection = 2;
		info.bIsLoop = true;
		info.Acceleration = Vector3(0.f, -25.f, 0.f);
		info.CreatePerSecond = 20.f;
		info.CreateTime = 5.f;
		info.GenerationRange = Vector3(10.f, 0.f, 10.f);
		info.InitialSpeed = Vector3(3.f, 0.f, 3.f);
		info.LifeTime = 2.0f;
		info.m_ParticleType = eParticleType::FIRE;
		info.m_ShaderPath = L"../Shader/Fire.hlsl";
		info.Size = Vector2(20.f, 20.f);
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_1.png";
		m_FireParticleComponentOne->Setting(info);
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_2.png";
		m_FireParticleComponentTwo->Setting(info);
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_3.png";
		m_FireParticleComponentThree->Setting(info);

		SetRootComponent(m_FireParticleComponentOne);
		m_FireParticleComponentOne->SetLocalPosition(Vector3(100.f, 0.f, -200.f));
		m_FireParticleComponentTwo->SetLocalPosition(Vector3(100.f, 0.f, -200.f));
		m_FireParticleComponentThree->SetLocalPosition(Vector3(100.f, 0.f, -200.f));
	}

	/// 연기 파티클 세팅 영역 ///
	{
		m_SmokeParticleComponentOne = CreateComponent<ParticleComponent>("SmokeParticleComponentOne").lock();
		m_SmokeParticleComponentTwo = CreateComponent<ParticleComponent>("SmokeParticleComponentTwo").lock();
		m_SmokeParticleComponentThree = CreateComponent<ParticleComponent>("SmokeParticleComponentThree").lock();
		m_SmokeParticleComponentFour = CreateComponent<ParticleComponent>("SmokeParticleComponentFour").lock();

		ParticleComponentInfo info;
		info.bIsLoop = true;
		info.Acceleration = Vector3(2.f, 8.f, 2.f);
		info.CreatePerSecond = 25.f;
		info.CreateTime = 3.f;
		info.GenerationRange = Vector3(15.f, 8.f, 15.f);
		info.InitialSpeed = Vector3(10.f, 30.f, 10.f);
		info.LifeTime = 8.0f;
		info.m_ParticleType = eParticleType::SMOKE;
		info.m_ShaderPath = L"../Shader/Smoke.hlsl";
		info.Size = Vector2(15.f, 15.f);
		info.m_TexturePath = L"../Resources/Textures/smoke_1.png";
		m_SmokeParticleComponentOne->Setting(info);
		info.CreatePerSecond = 10.f;
		info.m_TexturePath = L"../Resources/Textures/smoke_2.png";
		m_SmokeParticleComponentTwo->Setting(info);
		info.CreatePerSecond = 5.f;
		info.m_TexturePath = L"../Resources/Textures/smoke_3.png";
		m_SmokeParticleComponentThree->Setting(info);
		info.CreatePerSecond = 50.f;
		info.m_TexturePath = L"../Resources/Textures/smoke_4.png";
		m_SmokeParticleComponentFour->Setting(info);

		m_SmokeParticleComponentOne->SetLocalPosition(Vector3(200.f, 0.f, -200.f));
		m_SmokeParticleComponentTwo->SetLocalPosition(Vector3(200.f, 0.f, -200.f));
		m_SmokeParticleComponentThree->SetLocalPosition(Vector3(200.f, 0.f, -200.f));
		m_SmokeParticleComponentFour->SetLocalPosition(Vector3(200.f, 0.f, -200.f));
	}

	/// 반짝임 파티클 세팅 영역 ///
	{
		m_SparklesParticleComponentOne = CreateComponent<ParticleComponent>("SparklesParticleComponentOne").lock();
		m_SparklesParticleComponentTwo = CreateComponent<ParticleComponent>("SparklesParticleComponentTwo").lock();
		m_SparklesParticleComponentThree = CreateComponent<ParticleComponent>("SparklesParticleComponentThree").lock();

		ParticleComponentInfo info;
		info.bIsLoop = true;
		info.Acceleration = Vector3(0.f, 0.f, 0.f);
		info.CreatePerSecond = 10.f;
		info.CreateTime = 3.f;
		info.GenerationRange = Vector3(50.f, 0.f, 50.f);
		info.InitialSpeed = Vector3(1.f, 20.f, 1.f);
		info.LifeTime = 6.0f;
		info.m_ParticleType = eParticleType::SPARKLE;
		info.m_ShaderPath = L"../Shader/Sparkle.hlsl";
		info.Size = Vector2(12.f, 12.f);
		info.m_TexturePath = L"../Resources/Textures/sparkles_1.png";
		m_SparklesParticleComponentOne->Setting(info);
		info.CreatePerSecond = 50.f;
		info.m_TexturePath = L"../Resources/Textures/sparkles_2.png";
		m_SparklesParticleComponentTwo->Setting(info);
		info.GenerationRange = Vector3(40.f, 0.f, 40.f);
		info.Size = Vector2(8.f, 8.f);
		info.CreatePerSecond = 15.f;
		info.m_TexturePath = L"../Resources/Textures/sparkles_4.png";
		m_SparklesParticleComponentThree->Setting(info);

		m_SparklesParticleComponentOne->SetLocalPosition(Vector3(300.f, 0.f, -200.f));
		m_SparklesParticleComponentTwo->SetLocalPosition(Vector3(300.f, 0.f, -200.f));
		m_SparklesParticleComponentThree->SetLocalPosition(Vector3(300.f, 0.f, -200.f));
	}

	// 스켈레탈 메시 테스트
	{
		//m_TestSkeletalMeshComponent1 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent2 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent3 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent4 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent5 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent6 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent7 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent8 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent9 = CreateComponent<SkeletalMeshComponent>("1").lock();
		//m_TestSkeletalMeshComponent10 = CreateComponent<SkeletalMeshComponent>("1").lock();

		//m_TestSkeletalMeshComponent1.lock()->SetDefaultAnimation("../Resources/TestSkeletal/bear_ animation.fbx");
		//m_TestSkeletalMeshComponent2.lock()->SetDefaultAnimation("../Resources/TestSkeletal/bear_ animation_2.fbx");
		//m_TestSkeletalMeshComponent3.lock()->SetDefaultAnimation("../Resources/TestSkeletal/bear_ animation_3.fbx");
		//m_TestSkeletalMeshComponent4.lock()->SetDefaultAnimation("../Resources/TestSkeletal/bear_ head_.fbx");
		//m_TestSkeletalMeshComponent5.lock()->SetDefaultAnimation("../Resources/TestSkeletal/BEAR_ANI_BEAR_A.fbx");
		//m_TestSkeletalMeshComponent6.lock()->SetDefaultAnimation("../Resources/TestSkeletal/BEAR_ANI_BEAR_O.fbx");
		//m_TestSkeletalMeshComponent7.lock()->SetDefaultAnimation("../Resources/TestSkeletal/BEAR_ANI_BEAR_T.fbx");
		//m_TestSkeletalMeshComponent8.lock()->SetDefaultAnimation("../Resources/TestSkeletal/BEAR_ANI_BEAR_x.fbx");
		//m_TestSkeletalMeshComponent9.lock()->SetDefaultAnimation("../Resources/TestSkeletal/bear_model.fbx");
		//m_TestSkeletalMeshComponent10.lock()->SetDefaultAnimation("../Resources/TestSkeletal/bear_with_armature.fbx");

		//m_TestSkeletalMeshComponent1.lock()->SetLocalPosition(Vector3(0.f   , 1000.f, 0.f));
		//m_TestSkeletalMeshComponent2.lock()->SetLocalPosition(Vector3(100.f , 1000.f, 500.f));
		//m_TestSkeletalMeshComponent3.lock()->SetLocalPosition(Vector3(200.f , 1000.f, 0.f));
		//m_TestSkeletalMeshComponent4.lock()->SetLocalPosition(Vector3(300.f , 1000.f, 500.f));
		//m_TestSkeletalMeshComponent5.lock()->SetLocalPosition(Vector3(400.f , 1000.f, 0.f));
		//m_TestSkeletalMeshComponent6.lock()->SetLocalPosition(Vector3(500.f , 1000.f, 500.f));
		//m_TestSkeletalMeshComponent7.lock()->SetLocalPosition(Vector3(600.f , 1000.f, 0.f));
		//m_TestSkeletalMeshComponent8.lock()->SetLocalPosition(Vector3(700.f , 1000.f, 500.f));
		//m_TestSkeletalMeshComponent9.lock()->SetLocalPosition(Vector3(800.f , 1000.f, 0.f));
		//m_TestSkeletalMeshComponent10.lock()->SetLocalPosition(Vector3(900.f + 300.f, 1000.f, 500.f));
	}

	GameObject::Initialize();
}

void UIMeshTestObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}


/// MaterialComponent의 Material 변경 예제 코드 ///
void UIMeshTestObject::ChangedMaterial()
{
	m_MaterialManagerComponent->ChangedMaterial(m_SkeletalTestComponent, "TestMaterial", 0, true, 1.f);
}


/// MaterialComponent의 Material 변경 예제 코드 ///
void UIMeshTestObject::ChangedUIMaterial()
{
	m_MaterialManagerComponent->ChangedMaterial(m_PlayerOneMeshComponent, "TestMaterial", 1, true, 0.5f);
}


/// Particle Component의 파티클 재생 예제 코드 ///
void UIMeshTestObject::PlayParticle()
{
	m_ShockParticleComponentOne->PlayParticle(0.1f);
	m_ShockParticleComponentTwo->PlayParticle(0.1f);
	m_ShockParticleComponentThree->PlayParticle(0.1f);
	m_ShockParticleComponentFour->PlayParticle(0.1f);
}


/// OutLine On/Off 예제 코드 ///
void UIMeshTestObject::ChangedMeshOutLine()
{
	if (m_SkeletalTestComponent->GetIsOutLine() == true)
		m_SkeletalTestComponent->SetIsOutLine(false);
	else if (m_SkeletalTestComponent->GetIsOutLine() == false)
		m_SkeletalTestComponent->SetIsOutLine(true);
}
