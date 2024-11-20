#include "pch.h"
#include "ModelResource.h"

#include "NodeAnimation.h"
#include "Mesh.h"
#include "Material.h"
#include "Bone.h"
#include "SkeletonResource.h"


ModelResource::ModelResource()
{
}

ModelResource::~ModelResource()
{
	m_Meshes.clear();
	m_Materials.clear();
}

bool ModelResource::ReadFile(std::string filePath, ModelType modelType)
{
	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);

	unsigned int importFlags;
	if(modelType == ModelType::SKELETAL)
	{
		importFlags = aiProcess_Triangulate | // 삼각형으로 변환
			aiProcess_GenNormals |   // 노말 생성
			aiProcess_GenUVCoords |  // UV 생성
			aiProcess_CalcTangentSpace |  // 탄젠트 생성
			aiProcess_LimitBoneWeights |  // 본의 영향을 받는 정점의 최대 개수를 4개로 제한
			aiProcess_GenBoundingBoxes |  // 바운딩 박스 생성
			aiProcess_ConvertToLeftHanded;   // 왼손 좌표계로 변환
	}
	else
	{
		importFlags = aiProcess_Triangulate | // 삼각형으로 변환
			aiProcess_GenNormals |   // 노말 생성
			aiProcess_GenUVCoords |  // UV 생성
			aiProcess_CalcTangentSpace |  // 탄젠트 생성
			aiProcess_LimitBoneWeights |  // 본의 영향을 받는 정점의 최대 개수를 4개로 제한
			aiProcess_GenBoundingBoxes |  // 바운딩 박스 생성
			aiProcess_PreTransformVertices |  // 계층구조 노드들의 각각 포지션을 받아와서 적용된 상태로 함
			aiProcess_ConvertToLeftHanded;   // 왼손 좌표계로 변환
	}

	const aiScene* fbxModel = importer.ReadFile(filePath, importFlags);

	if (!fbxModel)
	{
		LOG_ERRORA("Error loading FBX file: %s", importer.GetErrorString());
		return false;
	}


	if (modelType == ModelType::SKELETAL)
		m_Skeleton.Create(fbxModel);

	// fbx에 적용된 맵(텍스쳐) 들을 가져오는 과정.
	m_Materials.resize(fbxModel->mNumMaterials);
	for (int i = 0; i < fbxModel->mNumMaterials; i++)
	{
		m_Materials[i].Create(fbxModel->mMaterials[i]);
	}

	// vertex , index 정보 바인딩
	m_Meshes.resize(fbxModel->mNumMeshes);
	for (int i = 0; i < fbxModel->mNumMeshes; i++)
	{
		aiMesh* fbxMesh = fbxModel->mMeshes[i];

		if (modelType == ModelType::STATIC)
		{
			m_Meshes[i].Create(fbxMesh);
		}
		else if (modelType == ModelType::SKELETAL)
		{
			m_Meshes[i].CreateBoneWeight(fbxMesh, &m_Skeleton);
		}
	}

	// SceneResource의 기본 애니메이션 추가한다.
	// ToDO :: 재현  애니메이션 개수가 여러개일 경우 처리해주기.
	 //assert(fbxModel->mNumAnimations < 2); // 애니메이션은 없거나 1개여야한다. 
	// 노드의 애니메이션을 하나로 합치는 방법은 FBX export에서 NLA스트립,모든 액션 옵션을 끕니다.
	if (fbxModel->HasAnimations())
	{
		const aiAnimation* pAiAnimation = fbxModel->mAnimations[0];
		// 채널수는 aiAnimation 안에서 애니메이션 정보를  표현하는 aiNode의 개수이다.
		assert(pAiAnimation->mNumChannels >= 1); // 애니메이션이 있다면 aiNode 는 하나 이상 있어야한다.

		shared_ptr<Animation> anim = make_shared<Animation>();
		anim->Create(filePath, pAiAnimation);
		m_Animations.push_back(anim);
	}

	float absMax = 0.f;
	for (UINT i = 0; i < fbxModel->mNumMeshes; i++)
	{
		aiMesh* pMesh = fbxModel->mMeshes[i];
		Math::Vector3 meshMin = Math::Vector3(pMesh->mAABB.mMin.x, pMesh->mAABB.mMin.y, pMesh->mAABB.mMin.z);
		Math::Vector3 meshMax = Math::Vector3(pMesh->mAABB.mMax.x, pMesh->mAABB.mMax.y, pMesh->mAABB.mMax.z);

		BoundingBox boundingBox;
		boundingBox.Center = (meshMin + meshMax) / 2;
		boundingBox.Extents = (meshMax - meshMin) / 2;
		m_InstanceBoundingBox.push_back(boundingBox);

		m_AABBmin = Math::Vector3::Min(m_AABBmin, meshMin);
		m_AABBmax = Math::Vector3::Max(m_AABBmax, meshMax);
	}
	absMax = max(m_AABBmax.Length(), m_AABBmin.Length());
	m_BoundingBoxMin = Math::Vector3(-absMax, -absMax, -absMax);
	m_BoundingBoxMax = Math::Vector3(absMax, absMax, absMax);

	importer.FreeScene();
	return true;
}

Material* ModelResource::GetMeshMaterial(UINT index)
{
	assert(index < m_Meshes.size());
	UINT mindex = m_Meshes[index].m_MaterialIndex;
	assert(mindex < m_Materials.size());
	return &m_Materials[mindex];
}

Math::Vector3 ModelResource::GetBoundingBoxDimensions() const
{
	return Math::Vector3(m_AABBmax.x - m_AABBmin.x, m_AABBmax.y - m_AABBmin.y, m_AABBmax.z - m_AABBmin.z);
}