#pragma once
#include "SkeletalMeshInstance.h"
#include "Transform.h"
#include "Bone.h"

#include <vector>
#include <list> 

class Material;
class ModelResource;
class SkeletonResource;

class SkeletalMeshComponent : public Transform
{
public:
	SkeletalMeshComponent();
	~SkeletalMeshComponent();

public:
	Bone m_RootNode;
	UINT  m_AnimationIndex = 0;
	float m_AnimationProgressTime = 0.0f;

	std::shared_ptr<ModelResource>  m_ModelResource;
	std::vector<SkeletalMeshInstance> m_MeshInstances;
	std::list<SkeletalMeshComponent*>::iterator m_iterator;

	std::string m_SceneFilePath; // BeginPlay에서 로딩
	std::list<std::string> m_AnimationFilePath; // BeginPlay에서 로딩	

	bool ReadSceneResourceFromFBX(std::string filePath);
	bool AddSceneAnimationFromFBX(std::string filePath);
	void SetSceneResource(std::shared_ptr<ModelResource> val);

	void UpdateBoneAnimationReference(UINT index);
	void PlayAnimation(UINT index);
	void CreateHierachy(SkeletonResource* skeleton);

	Material* GetMaterial(UINT index);

	virtual void Update(float DeltaTime) override;
	virtual void OnBeginPlay();
	virtual void OnEndPlay();	

};