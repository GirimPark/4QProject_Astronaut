#include "pch.h"
#include "ShockParticleObjcect.h"

#include "../Engine/ParticleComponent.h"
#include "../Engine/EventManager.h"

ShockParticleObjcect::ShockParticleObjcect()
{
	EventManager::GetInstance()->RegisterListener(eEventType::PLAY_CRASH_PARTICLE, this);
}

ShockParticleObjcect::~ShockParticleObjcect()
{
	EventManager::GetInstance()->UnregisterListener(eEventType::PLAY_CRASH_PARTICLE, this);
}

void ShockParticleObjcect::PlayParticle()
{
	m_ShockParticleComponentOne->PlayParticle(0.1f);
	m_ShockParticleComponentTwo->PlayParticle(0.1f);
	m_ShockParticleComponentThree->PlayParticle(0.1f);
	m_ShockParticleComponentFour->PlayParticle(0.1f);
}

void ShockParticleObjcect::Initialize()
{
	auto root = CreateComponent<SceneComponent>("RootComponent");
	SetRootComponent(root.lock());

	m_ShockParticleComponentOne = CreateComponent<ParticleComponent>("ShockParticleComponentOne").lock();
	m_ShockParticleComponentTwo = CreateComponent<ParticleComponent>("ShockParticleComponentTwo").lock();
	m_ShockParticleComponentThree = CreateComponent<ParticleComponent>("ShockParticleComponentThree").lock();
	m_ShockParticleComponentFour = CreateComponent<ParticleComponent>("ShockParticleComponentFour").lock();

	ParticleComponentInfo info;
	info.bIsLoop = false;
	info.Acceleration = Vector3(0.f, -450.f, 0.f);
	info.CreatePerSecond = 50.f;
	info.CreateTime = 0.2f;
	info.GenerationRange = Vector3(0.f, 0.f, 0.f);
	info.InitialSpeed = Vector3(300.f, 300.f, 300.f);
	info.LifeTime = 1.f;
	info.m_ParticleType = eParticleType::SHOCK_STAR;
	info.m_ShaderPath = L"../Shader/ShockStar.hlsl";
	info.Size = Vector2(30.f, 30.f);
	info.m_TexturePath = L"../Resources/Textures/Particle_star_1.png";
	m_ShockParticleComponentOne->Setting(info);
	info.m_TexturePath = L"../Resources/Textures/Particle_star_2.png";
	m_ShockParticleComponentTwo->Setting(info);
	info.CreatePerSecond = 30.f;
	info.Acceleration = Vector3(0.f, -600.f, 0.f);
	info.InitialSpeed = Vector3(450.f, 450.f, 450.f);
	info.LifeTime = 0.6f;
	info.Size = Vector2(24.f, 24.f);
	info.m_TexturePath = L"../Resources/Textures/Particle_star_1.png";
	m_ShockParticleComponentThree->Setting(info);
	info.Size = Vector2(15.f, 15.f);
	info.m_TexturePath = L"../Resources/Textures/Particle_star_2.png";
	m_ShockParticleComponentFour->Setting(info);

	__super::Initialize();
}

void ShockParticleObjcect::Update(float deltaTime)
{
	__super::Update(deltaTime);
}

void ShockParticleObjcect::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::PLAY_CRASH_PARTICLE)
	{
		m_ShockParticleComponentOne->SetLocalPosition(event->GetData<Vector3>());
		m_ShockParticleComponentTwo->SetLocalPosition(event->GetData<Vector3>());
		m_ShockParticleComponentThree->SetLocalPosition(event->GetData<Vector3>());
		m_ShockParticleComponentFour->SetLocalPosition(event->GetData<Vector3>());

		PlayParticle();
	}
}
