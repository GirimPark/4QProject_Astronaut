#include "pch.h"
#include "FireParticleObject.h"

#include "../Engine/ParticleComponent.h"
#include "../Engine/EventManager.h"

FireParticleObject::FireParticleObject()
{
	EventManager::GetInstance()->RegisterListener(eEventType::PLAY_FIRE_PARTICLE, this);
}

FireParticleObject::~FireParticleObject()
{
	EventManager::GetInstance()->UnregisterListener(eEventType::PLAY_FIRE_PARTICLE, this);
}

void FireParticleObject::PlayParticle(bool isPlaying)
{
	m_FireParticleComponentOne->PlayParticle(isPlaying);
	m_FireParticleComponentTwo->PlayParticle(isPlaying);
	m_FireParticleComponentThree->PlayParticle(isPlaying);
}

void FireParticleObject::SetLocalPositions(Math::Vector3 pos)
{
	m_FireParticleComponentOne->SetLocalPosition(pos);
	m_FireParticleComponentTwo->SetLocalPosition(pos);
	m_FireParticleComponentThree->SetLocalPosition(pos);
}

void FireParticleObject::Initialize()
{
	auto root = CreateComponent<SceneComponent>("RootComponent");
	SetRootComponent(root.lock());

	m_FireParticleComponentOne = CreateComponent<ParticleComponent>("FireParticleComponent1").lock();
	m_FireParticleComponentTwo = CreateComponent<ParticleComponent>("FireParticleComponent2").lock();
	m_FireParticleComponentThree = CreateComponent<ParticleComponent>("FireParticleComponent3").lock();

	ParticleComponentInfo info;
	info.ParticleDirection = 2;
	info.bIsLoop = true;
	info.Acceleration = Vector3(0.f, -1000.f, 0.f);
	info.CreatePerSecond = 300.f;
	info.CreateTime = 5.f;
	info.GenerationRange = Vector3(10.f, 0.f, 10.f);
	info.InitialSpeed = Vector3(2.f, 0.f, 2.f);
	info.LifeTime = 0.01f;
	info.m_ParticleType = eParticleType::FIRE;
	info.m_ShaderPath = L"../Shader/Fire.hlsl";
	info.Size = Vector2(45.f, 45.f);
	info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_3.png";
	m_FireParticleComponentOne->Setting(info);
	info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_2.png";
	m_FireParticleComponentTwo->Setting(info);
	info.m_TexturePath = L"../Resources/Textures/Stylized_Fire_particle_texture_3.png";
	m_FireParticleComponentThree->Setting(info);

	GameObject::Initialize();

	m_FireParticleComponentOne->SetLocalPosition({ 0.f, -180.f, 0.f });
	m_FireParticleComponentTwo->SetLocalPosition({ 0.f, -180.f, 0.f });
	m_FireParticleComponentThree->SetLocalPosition({ 0.f, -180.f, 0.f });
}

void FireParticleObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void FireParticleObject::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::PLAY_FIRE_PARTICLE)
	{
		//m_FireParticleComponentOne->SetLocalPosition({ 0.f, -40.f, 0.f });
		//m_FireParticleComponentTwo->SetLocalPosition({ 0.f, -40.f, 0.f });
		//m_FireParticleComponentThree->SetLocalPosition({ 0.f, -40.f, 0.f });

		PlayParticle(event->GetData<bool>());
	}
}
