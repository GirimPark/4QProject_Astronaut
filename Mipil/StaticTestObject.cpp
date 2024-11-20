#include "pch.h"
#include "StaticTestObject.h"

#include "../Engine/StaticMeshComponent.h"
#include "../Engine/ParticleComponent.h"

void StaticTestObject::Initialize()
{
	m_meshComponent = CreateComponent<StaticMeshComponent>("StaticMeshComponent");
	//m_meshComponent.lock()->SetFilePath("../Resources/FBX/zeldaPosed001.fbx");
	//m_meshComponent.lock()->SetFilePath("../Resources/FBX/cerberus2.fbx");
	m_meshComponent.lock()->SetFilePath("../Resources/FBX/sphere_alpha3.fbx");
	SetRootComponent(m_meshComponent.lock());

	// 파티클 생성 예제
	{
		m_ParticleComponent1 = CreateComponent<ParticleComponent>("ParticleComponent");
		ParticleComponentInfo info;
		info.Acceleration = Vector3(0.f, -100.f, 0.f);		// 가속도
		info.CreatePerSecond = 10.f;						// 1초당 생성 갯수
		info.CreateTime = 5.f;								// 생성 시간 ( 미구현 )
		info.GenerationRange = Vector3(10.f, 0.f, 10.f);	// 초기 위치 범위
		info.InitialSpeed = Vector3(0.f, -2.f, 0.f);		// 초기 속도
		info.Size = Vector2(30.f, 30.f);					// 파티클 크기
		info.LifeTime = 1.f;								// 유지 시간
		info.m_ParticleType = eParticleType::FIRE;
		info.m_ShaderPath = L"../Shader/Fire.hlsl";
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_1.png";
		m_ParticleComponent1.lock()->SetVisible(true);
		m_ParticleComponent1.lock()->Setting(info);
		m_ParticleComponent1.lock()->SetLocalPosition(Vector3(200.f, 0.f, 200.f));
	}
	{
		m_ParticleComponent2 = CreateComponent<ParticleComponent>("ParticleComponent");
		ParticleComponentInfo info;
		info.Acceleration = Vector3(0.f, -100.f, 0.f);		// 가속도
		info.CreatePerSecond = 10.f;						// 1초당 생성 갯수
		info.CreateTime = 5.f;								// 생성 시간 ( 미구현 )
		info.GenerationRange = Vector3(10.f, 0.f, 10.f);	// 초기 위치 범위
		info.InitialSpeed = Vector3(0.f, -2.f, 0.f);		// 초기 속도
		info.Size = Vector2(30.f, 30.f);					// 파티클 크기
		info.LifeTime = 1.f;								// 유지 시간
		info.m_ParticleType = eParticleType::FIRE;
		info.m_ShaderPath = L"../Shader/Fire.hlsl";
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_2.png";
		m_ParticleComponent2.lock()->SetVisible(true);
		m_ParticleComponent2.lock()->Setting(info);
		m_ParticleComponent2.lock()->SetLocalPosition(Vector3(200.f, 0.f, 200.f));
	}
	{
		m_ParticleComponent3 = CreateComponent<ParticleComponent>("ParticleComponent");
		ParticleComponentInfo info;
		info.Acceleration = Vector3(0.f, -100.f, 0.f);		// 가속도
		info.CreatePerSecond = 10.f;						// 1초당 생성 갯수
		info.CreateTime = 5.f;								// 생성 시간 ( 미구현 )
		info.GenerationRange = Vector3(10.f, 0.f, 10.f);	// 초기 위치 범위
		info.InitialSpeed = Vector3(0.f, -2.f, 0.f);		// 초기 속도
		info.Size = Vector2(30.f, 30.f);					// 파티클 크기
		info.LifeTime = 1.f;								// 유지 시간
		info.m_ParticleType = eParticleType::FIRE;
		info.m_ShaderPath = L"../Shader/Fire.hlsl";
		info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_3.png";
		m_ParticleComponent3.lock()->SetVisible(true);
		m_ParticleComponent3.lock()->Setting(info);
		m_ParticleComponent3.lock()->SetLocalPosition(Vector3(200.f, 0.f, 200.f));
	}

	GameObject::Initialize();
}

void StaticTestObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
