#pragma once
#include "SkeletalMeshInstance.h"
#include "Bone.h"

class Material;
class ModelResource;
class SkeletonResource;

class SkeletalMeshModel
{
public:
	SkeletalMeshModel();
	~SkeletalMeshModel();

public:
	Material* GetMaterial(UINT index);
	double GetAnimationDuration(int animationIdx);
	void SetIsOutLine(bool isOutLine) { m_IsOutLine = isOutLine; }
	void SetIsMove(bool isMove) { m_IsUIMove = isMove; }
	void SetWorldMatrix(Math::Matrix* worldMatrix) { m_worldMatrix = worldMatrix; }
	void SetAnimationTime(float* progressTime) { m_pAnimationProgressTime = progressTime; }
	void SetInstanceMaterial(Material* material, int instanceNumber);

	bool ReadSceneResourceFromFBX(std::string filePath);
	bool AddSceneAnimationFromFBX(std::string filePath, bool isLoop);
	void PlayAnimation(UINT index);
	void UpdateBoneNode(float deltaTime);
	void ChangeBoneAnimationReference(UINT index);
	void SettingBindposeMatrix();

private:
	void SetSceneResource(std::shared_ptr<ModelResource> val);
	void InitBoneAnimationReference(UINT index);
	void CreateHierachy(SkeletonResource* skeleton);

	//void Update(float DeltaTime) override;

public:
	bool m_IsCulled = true;
	bool m_IsOutLine = false;
	bool m_IsUIMove = false;
	bool* m_isLoop;

	Bone m_RootNode;
	float* m_pAnimationProgressTime = nullptr;
	Math::Matrix* m_worldMatrix = nullptr;	// meshComponent의 matrix 참조

	BoundingBox m_BoundingBox;

	std::shared_ptr<ModelResource>  m_ModelResource;
	std::vector<SkeletalMeshInstance> m_MeshInstances;
	//std::list<SkeletalMeshModel*>::iterator m_iterator;

	//std::string m_SceneFilePath; // BeginPlay에서 로딩
	//std::list<std::string> m_AnimationFilePath; // BeginPlay에서 로딩

};