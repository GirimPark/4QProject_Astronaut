#include "pch.h"
#include "StaticMeshModel.h"
#include "D3DRenderManager.h"
#include "ResourceManager.h"
#include "ModelResource.h"

StaticMeshModel::StaticMeshModel()
{
}

StaticMeshModel::~StaticMeshModel()
{
	m_ModelResource.reset();
	m_MeshInstances.clear();
}

bool StaticMeshModel::ReadSceneResourceFromFBX(std::string filePath)
{
	// FBX 파일 읽기
	std::shared_ptr<ModelResource> sceneResource = ResourceManager::GetInstance()->CreateModelResource(filePath, ModelType::STATIC);
	if (sceneResource == nullptr)
	{
		return false;
	}
	SetSceneResource(sceneResource);
	return true;
}

void StaticMeshModel::SetSceneResource(std::shared_ptr<ModelResource> val)
{
	assert(val);
	assert(m_worldMatrix);

	m_ModelResource = val;
	// 인스턴스 생성
	m_MeshInstances.resize(m_ModelResource->m_Meshes.size());
	for (UINT i = 0; i < m_ModelResource->m_Meshes.size(); i++)
	{
		m_MeshInstances[i].Create(&m_ModelResource->m_Meshes[i], // mesh resource
			m_worldMatrix,	// root node
			m_ModelResource->GetMeshMaterial(i),
			&m_ModelResource->m_InstanceBoundingBox[i]
		);		//material resource
	}

	m_BoundingBox.Center = Math::Vector3(m_ModelResource->m_AABBmin + m_ModelResource->m_AABBmax) * 0.5f;	// Calculate extent
	m_BoundingBox.Extents = Math::Vector3(m_ModelResource->m_AABBmax - m_ModelResource->m_AABBmin);	// Calculate extent

	float maxDistance = max(m_BoundingBox.Extents.x, max(m_BoundingBox.Extents.y, m_BoundingBox.Extents.z));
	m_BoundingBox.Extents.x = maxDistance;
	m_BoundingBox.Extents.y = maxDistance;
	m_BoundingBox.Extents.z = maxDistance;
}

Material* StaticMeshModel::GetMaterial(UINT index)
{
	assert(index < m_ModelResource->m_Materials.size());
	return &m_ModelResource->m_Materials[index];
}

const std::vector<Mesh>& StaticMeshModel::GetMeshes()
{
	return m_ModelResource->m_Meshes;
}