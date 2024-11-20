#include "pch.h"
#include "ResourceManager.h"
#include "D3DRenderManager.h"
#include "SkeletalMeshModel.h"
#include "SkeletalMeshInstance.h"
#include "ModelResource.h"
#include "Bone.h"
#include "NodeAnimation.h"
#include "SkeletonResource.h"

SkeletalMeshModel::SkeletalMeshModel()
{
}

SkeletalMeshModel::~SkeletalMeshModel()
{
}

void SkeletalMeshModel::SetInstanceMaterial(Material* material, int instanceNumber)
{
	m_MeshInstances[instanceNumber].m_pMaterial = material;
}

bool SkeletalMeshModel::ReadSceneResourceFromFBX(std::string filePath)
{
	 //FBX 파일 읽기
	std::shared_ptr<ModelResource> sceneResource = ResourceManager::GetInstance()->CreateModelResource(filePath, ModelType::SKELETAL);
	if (sceneResource == nullptr)
	{
		return false;
	}
	SetSceneResource(sceneResource);
	return true;
}

void SkeletalMeshModel::SetSceneResource(std::shared_ptr<ModelResource> val)
{
	assert(val);
	m_ModelResource = val;

	CreateHierachy(&m_ModelResource->m_Skeleton);

	// 인스턴스 생성
	m_MeshInstances.resize(m_ModelResource->m_Meshes.size());
	for (UINT i = 0; i < m_ModelResource->m_Meshes.size(); i++)
	{
		Mesh* meshResource = &m_ModelResource->m_Meshes[i];
		Material* material = m_ModelResource->GetMeshMaterial(i);
		m_MeshInstances[i].Create(meshResource, &m_RootNode, material);
	}
	InitBoneAnimationReference(0);	// 각 노드의 애니메이션 정보참조 연결

	m_BoundingBox.Center = Math::Vector3(m_ModelResource->m_AABBmin + m_ModelResource->m_AABBmax) * 0.5f;	// Calculate extent
	m_BoundingBox.Extents = Math::Vector3(m_ModelResource->m_AABBmax - m_ModelResource->m_AABBmin);	// Calculate extent
}

void SkeletalMeshModel::ChangeBoneAnimationReference(UINT index)
{
	assert(index < m_ModelResource->m_Animations.size());
	auto animation = m_ModelResource->m_Animations[index];
	*m_isLoop = animation->m_animLoop;
	*m_pAnimationProgressTime = 0.0f;

	for (size_t i = 0; i < animation->m_NodeAnims.size(); i++)
	{
		NodeAnimation& nodeAnimation = animation->m_NodeAnims[i];
		Bone* pBone = m_RootNode.FindNode(nodeAnimation.m_NodeName);
		nodeAnimation.m_bIsLoop = m_isLoop;

		assert(pBone != nullptr);
		pBone->m_pNextNodeAnimation = &animation->m_NodeAnims[i];
	}
}

void SkeletalMeshModel::SettingBindposeMatrix()
{
	// 여기에선 애니메이션이 바뀔 경우 안쓸 본의 행렬 초기화 작업.
	// Setting Bindpose Matrix
	SkeletonResource* pSkeleton = &m_ModelResource->m_Skeleton;
	for (size_t i = 0; i < pSkeleton->Bones.size(); i++)
	{
		BoneInfo* pBoneInfo = pSkeleton->GetBone(i);
		Bone* pBone = m_RootNode.FindNode(pBoneInfo->Name);

		assert(pBone != nullptr);
		if (pBone->m_pNextNodeAnimation == nullptr)
		{
			pBone->m_pCurNodeAnimation = nullptr;
			pBone->m_localTransform = pBoneInfo->RelativeTransform; // T포즈
		}
	}
}

void SkeletalMeshModel::InitBoneAnimationReference(UINT index)
{
	assert(index < m_ModelResource->m_Animations.size());
	auto animation = m_ModelResource->m_Animations[index];
	for (size_t i = 0; i < animation->m_NodeAnims.size(); i++)
	{
		NodeAnimation& nodeAnimation = animation->m_NodeAnims[i];
		nodeAnimation.m_bIsLoop = m_isLoop;
		Bone* pBone = m_RootNode.FindNode(nodeAnimation.m_NodeName);
		assert(pBone != nullptr);
		pBone->m_pCurNodeAnimation = &animation->m_NodeAnims[i];
	}
}

void SkeletalMeshModel::PlayAnimation(UINT index)
{
	assert(index < m_ModelResource->m_Animations.size());
	InitBoneAnimationReference(index);
}

void SkeletalMeshModel::UpdateBoneNode(float deltaTime)
{
	m_RootNode.Update(deltaTime);
}


void SkeletalMeshModel::CreateHierachy(SkeletonResource* skeleton)
{
	UINT count = skeleton->GetBoneCount();

	BoneInfo* pRootBone = skeleton->GetBone(0);
	m_RootNode.m_name = pRootBone->Name;
	m_RootNode.m_Children.reserve(pRootBone->NumChildren);
	m_RootNode.SetParentMoel(this);

	// 0번 루트는 컨테이너이므로 현재 Node와 같다 그러므로 1번부터 시작한다.
	for (UINT i = 1; i < count; i++)
	{
		BoneInfo* pBoneInfo = skeleton->GetBone(i);
		assert(pBoneInfo != nullptr);
		assert(pBoneInfo->ParentBoneIndex != -1);

		Bone* pParentBone = m_RootNode.FindNode(skeleton->GetBoneName(pBoneInfo->ParentBoneIndex));
		assert(pParentBone != nullptr);

		auto& ChildBone = pParentBone->CreateChild();
		ChildBone.m_name = pBoneInfo->Name;
		ChildBone.m_localTransform = pBoneInfo->RelativeTransform;
		ChildBone.m_Children.reserve(pBoneInfo->NumChildren);
		ChildBone.m_pParentBone = pParentBone;
		ChildBone.m_pAnimationTime = m_pAnimationProgressTime;
	}
}

bool SkeletalMeshModel::AddSceneAnimationFromFBX(std::string filePath, bool animLoop)
{
	assert(m_ModelResource);
	auto animation = ResourceManager::GetInstance()->CreateAnimationResource(filePath, animLoop);
	if (!animation) {
		return false;
	}

	//m_AnimationFilePath.push_back(filePath);
	m_ModelResource->m_Animations.push_back(animation);
	return true;
}
Material* SkeletalMeshModel::GetMaterial(UINT index)
{
	assert(index < m_ModelResource->m_Materials.size());
	return &m_ModelResource->m_Materials[index];
}

double SkeletalMeshModel::GetAnimationDuration(int animationIdx)
{
	return m_ModelResource->m_Animations[animationIdx]->m_Duration;
}