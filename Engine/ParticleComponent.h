#pragma once
#include "RenderComponent.h"

#include "../D3DRenderer/ParticleSystem.h"

/// Particle Component에 필요한 데이터 구조체 ///
struct ParticleComponentInfo
{
	Vector3 InitialSpeed;			// 파티클 초기 속도
	Vector3 Acceleration;			// 파티클 가속도
	Vector2 Size;					// 파티클 크기
	Vector3 GenerationRange;		// 파티클 생성 범위
	float CreatePerSecond;			// 파티클 1초당 생성 갯수
	float LifeTime;					// 파티클 생존 시간
	float CreateTime;				// 파티클 생성 시간
	std::wstring m_ShaderPath;		// 파티클 셰이더 경로
	std::wstring m_TexturePath;		// 파티클 텍스쳐 경로
	eParticleType m_ParticleType;	// 파티클 타입
	bool bIsLoop = true;			// 파티클 루프
	int ParticleDirection = 0;		// 파티클 방향
};


/// Particle Component ///
class ParticleComponent :
    public RenderComponent
{
public:
	ParticleComponent() = default;
	~ParticleComponent() override;

public:
	std::shared_ptr<ParticleSystem> GetParticle() const { return m_pParticleSystem; }

	// CreateComponent 후 필수적으로 filePath 설정
	void Setting(ParticleComponentInfo particleInfo) { m_ParticleInfo = particleInfo; }


public:
	void Initialize() override;	// staticMeshModel을 렌더러를 통해 생성, 받아온다.
	void Update(float deltaTime) override;

public:
	void PlayParticle(float particlePlayTime);
	void PlayParticle(bool isPlaying);

private:
	ParticleComponentInfo m_ParticleInfo;				// 파티클 정보
	std::shared_ptr<ParticleSystem> m_pParticleSystem;

	bool m_bIsLoop;
	float m_DurationTime;
	float m_ParticlePlayTime;
};