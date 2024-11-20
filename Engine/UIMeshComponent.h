#pragma once
#include "RenderComponent.h"

class SkeletalMeshModel;
class Material;

class UIMeshComponent : public RenderComponent
{
public:
	UIMeshComponent() = default;
	~UIMeshComponent() override;

public:
	std::shared_ptr<SkeletalMeshModel> GetSkeletalMeshModel() const { return m_skeletalMeshModel; }
	int GetPlayerNumber() const { return m_PlayerNumber; }

	// CreateComponent 후 필수적으로 filePath 설정
	void SetPlaySpeed(float speed) { m_animationPlaySpeed = speed; }
	void SetPlayerNumber(int playerNumber) { m_PlayerNumber = playerNumber; }
	void SetOutLine(bool isOutLine);
	void SetUIMove(bool isUIMove);
	Material* SetMaterial(Material* material, int instanceNumber);

	void SetDefaultAnimation(std::string_view filePath, bool isLoop = true);
	void ChangeAnimation(std::string name);
	void AddAnimation(std::string filePath, bool isLoop);

public:
	void Initialize() override;	// SkeletalMeshModel을 렌더러를 통해 생성, 받아온다.
	void Update(float deltaTime) override;


private:
	std::string m_filePath;
	std::shared_ptr<SkeletalMeshModel> m_skeletalMeshModel;

	bool m_isLoop = true;
	int  m_animationIndex = 0;
	float m_animationProgressTime = 0.0f;
	float m_animationPlaySpeed = 1.f;
	std::unordered_map<std::string, int> m_animationMap;

	// for anim change
	float m_durationTime = 0.0f;
	bool m_bAnimChanged = false;

	// UI용 PlayerNumber
	int m_PlayerNumber = 0;
};