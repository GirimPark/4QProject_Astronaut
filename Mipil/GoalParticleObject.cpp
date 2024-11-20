#include "pch.h"
#include "GoalParticleObject.h"

#include "../Engine/ParticleComponent.h"

GoalParticleObject::GoalParticleObject()
{
}

GoalParticleObject::~GoalParticleObject()
{
}

void GoalParticleObject::SetGoalPosition(int x, int y, int z)
{
	m_SparklesParticleComponentOne->SetLocalPosition(Vector3(180.f * x + 90.f, 180.f * y, 180.f * z + 90.f));
	m_SparklesParticleComponentTwo->SetLocalPosition(Vector3(180.f * x + 90.f, 180.f * y, 180.f * z + 90.f));
	m_SparklesParticleComponentThree->SetLocalPosition(Vector3(180.f * x + 90.f, 180.f * y, 180.f * z + 90.f));
}

void GoalParticleObject::Initialize()
{
	auto root = CreateComponent<SceneComponent>("RootComponent");
	SetRootComponent(root.lock());

	{
		m_SparklesParticleComponentOne = CreateComponent<ParticleComponent>("SparklesParticleComponentOne").lock();
		m_SparklesParticleComponentTwo = CreateComponent<ParticleComponent>("SparklesParticleComponentTwo").lock();
		m_SparklesParticleComponentThree = CreateComponent<ParticleComponent>("SparklesParticleComponentThree").lock();

		ParticleComponentInfo info;
		info.bIsLoop = true;
		info.Acceleration = Vector3(0.f, 0.f, 0.f);
		info.CreatePerSecond = 20.f;
		info.CreateTime = 3.f;
		info.GenerationRange = Vector3(180.f, 0.f, 180.f);
		info.InitialSpeed = Vector3(1.f, 60.f, 1.f);
		info.LifeTime = 8.0f;
		info.m_ParticleType = eParticleType::SPARKLE;
		info.m_ShaderPath = L"../Shader/Sparkle.hlsl";
		info.Size = Vector2(30.f, 30.f);
		info.m_TexturePath = L"../Resources/Textures/sparkles_1.png";
		m_SparklesParticleComponentOne->Setting(info);
		info.CreatePerSecond = 100.f;
		info.m_TexturePath = L"../Resources/Textures/sparkles_2.png";
		m_SparklesParticleComponentTwo->Setting(info);
		info.GenerationRange = Vector3(140.f, 0.f, 140.f);
		info.Size = Vector2(20.f, 20.f);
		info.CreatePerSecond = 30.f;
		info.m_TexturePath = L"../Resources/Textures/sparkles_4.png";
		m_SparklesParticleComponentThree->Setting(info);

		m_SparklesParticleComponentOne->SetLocalPosition(Vector3(250.f * 3 + 65.f, 200.f, 90.f));
		m_SparklesParticleComponentTwo->SetLocalPosition(Vector3(250.f * 3 + 65.f, 200.f, 90.f));
		m_SparklesParticleComponentThree->SetLocalPosition(Vector3(250.f * 3 + 65.f, 200.f, 90.f));
	}

	GameObject::Initialize();
}

void GoalParticleObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
