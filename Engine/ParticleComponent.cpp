#include "pch.h"
#include "ParticleComponent.h"

#include "CommonApp.h"
#include "../D3DRenderer/ParticleSystem.h"

ParticleComponent::~ParticleComponent()
{
}


/// Particle Component 세팅 ///
void ParticleComponent::Initialize()
{
	m_DurationTime = 0;
	m_ParticlePlayTime = m_ParticleInfo.CreateTime;
	m_bIsLoop = m_ParticleInfo.bIsLoop;

	m_pParticleSystem = std::make_shared<ParticleSystem>();
	m_pParticleSystem->SetPlayParticle(m_ParticleInfo.bIsLoop);
	m_pParticleSystem->SetEmitPos(GetWorldPosition());
	m_pParticleSystem->SetParticleAcceleration(m_ParticleInfo.Acceleration);
	m_pParticleSystem->SetParticleCreatePerSecond(m_ParticleInfo.CreatePerSecond);
	m_pParticleSystem->SetParticleInitialSpeed(m_ParticleInfo.InitialSpeed);
	m_pParticleSystem->SetParticleLifeTime(m_ParticleInfo.LifeTime);
	m_pParticleSystem->SetParticleSize(m_ParticleInfo.Size);
	m_pParticleSystem->SetEmitGenerationRange(m_ParticleInfo.GenerationRange);
	m_pParticleSystem->SetParticleDirection(m_ParticleInfo.ParticleDirection);
	m_pParticleSystem->Init(m_ParticleInfo.m_TexturePath
		, m_ParticleInfo.CreatePerSecond * m_ParticleInfo.LifeTime * 1.5f
		, m_ParticleInfo.m_ParticleType
		, m_ParticleInfo.m_ShaderPath);
}

/// Particle Component Update ///
void ParticleComponent::Update(float deltaTime)
{
	RenderComponent::Update(deltaTime);

	m_pParticleSystem->SetEmitPos(GetWorldPosition());
	m_DurationTime += deltaTime;

	// 루프가 아니면 재생 시간이 넘어가면 파티클 플레이 중지
	if (!m_bIsLoop && m_ParticlePlayTime <= m_DurationTime)
		m_pParticleSystem->SetPlayParticle(false);

	if (m_bVisible)
		CommonApp::m_pInstance->GetRenderer()->AddParticle(m_pParticleSystem);
}


/// 해당 Particle이 IsLoop가 아닐 때 파티클 실행 함수 ///
void ParticleComponent::PlayParticle(float particlePlayTime)
{
	m_DurationTime = 0.f;
	m_ParticlePlayTime = particlePlayTime;
	m_pParticleSystem->SetPlayParticle(true);
	m_pParticleSystem->Reset();
}


/// Particle Play, Stop 함수 ///
void ParticleComponent::PlayParticle(bool isPlaying)
{
	m_pParticleSystem->SetPlayParticle(isPlaying);
	m_pParticleSystem->Reset();
}