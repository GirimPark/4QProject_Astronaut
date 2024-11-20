#pragma once
#include "RenderComponent.h"

class SkeletalMeshModel;
class Material;

/// <summary>
///	SkeletalMesh를 렌더링하기 위한 컴포넌트
///	Renderer의 SkeletalMeshModel을 만들고 소유한다.
///	애니메이션을 위한 추가 정보, 처리가 들어간다.
/// </summary>
class SkeletalMeshComponent
	: public RenderComponent
{
public:
	SkeletalMeshComponent() = default;
	~SkeletalMeshComponent() override;

public:
	std::shared_ptr<SkeletalMeshModel> GetSkeletalMeshModel() const { return m_skeletalMeshModel; }

	// CreateComponent 후 필수적으로 filePath 설정
	void SetPlaySpeed(float speed) { m_animationPlaySpeed = speed; }
	void SetIsOutLine(bool isOutLine);
	void SetOffsetPosition(Math::Vector3 offsetPos) { m_offsetPosition = offsetPos; }
	void SetOffsetRotation(Math::Vector3 offsetRot) { m_offsetRotation = offsetRot; }


	Material* SetMaterial(Material* material, int instanceNumber);
	bool GetIsOutLine();

	void SetDefaultAnimation(std::string_view filePath, bool isLoop = true);
	void ChangeAnimation(std::string name);
	void AddAnimation(std::string filePath, bool isLoop);

public:
	void Initialize() override;	// SkeletalMeshModel을 렌더러를 통해 생성, 받아온다.
	void Update(float deltaTime) override;

private:
	void SetOffsetMatrix();

private:
	std::string m_filePath;
	std::shared_ptr<SkeletalMeshModel> m_skeletalMeshModel;

	Math::Vector3 m_offsetPosition;
	Math::Vector3 m_offsetRotation;

	Math::Matrix m_offsetMatrix;
	Math::Matrix m_relativeMatrix;

	bool m_isLoop = true;
	int  m_animationIndex = 0;
	float m_animationProgressTime = 0.0f;
	float m_animationPlaySpeed = 1.f;
	std::unordered_map<std::string, int> m_animationMap;

	// for anim change
	float m_durationTime = 0.0f;
	bool m_bAnimChanged = false;
};

