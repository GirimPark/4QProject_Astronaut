#include "pch.h"
#include "StaticMeshComponent.h"
#include "D3DRenderManager.h"
#include "ResourceManager.h"
#include "ModelResource.h"

StaticMeshComponent::StaticMeshComponent()
{
	D3DRenderManager::Instance->m_StaticMeshComponents.push_back(this);
	m_iterator = --D3DRenderManager::Instance->m_StaticMeshComponents.end();
}

StaticMeshComponent::~StaticMeshComponent()
{
//	D3DRenderManager::GetInstance()->m_StaticMeshComponents.erase(m_iterator);
}

void StaticMeshComponent::Update(float DeltaTime)
{
	__super::Update(DeltaTime);
}

bool StaticMeshComponent::ReadSceneResourceFromFBX(std::string filePath)
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

void StaticMeshComponent::SetSceneResource(std::shared_ptr<ModelResource> val)
{
	assert(val);
	m_ModelResource = val;
	// 인스턴스 생성
	m_MeshInstances.resize(m_ModelResource->m_Meshes.size());
	for (UINT i = 0; i < m_ModelResource->m_Meshes.size(); i++)
	{
		m_MeshInstances[i].Create(&m_ModelResource->m_Meshes[i], // mesh resource		
			&m_World,	// root node
			m_ModelResource->GetMeshMaterial(i));		//material resource 
	}

//	m_BoundingBox.Center = Math::Vector3(m_ModelResource->m_AABBmin + m_ModelResource->m_AABBmax) * 0.5f;	// Calculate extent
//	m_BoundingBox.Extents = Math::Vector3(m_ModelResource->m_AABBmax - m_ModelResource->m_AABBmin);	// Calculate extent
}

Material* StaticMeshComponent::GetMaterial(UINT index)
{
	assert(index < m_ModelResource->m_Materials.size());
	return &m_ModelResource->m_Materials[index];
}

void StaticMeshComponent::OnBeginPlay()
{
	if (!m_SceneFilePath.empty())
		ReadSceneResourceFromFBX(m_SceneFilePath);
}

void StaticMeshComponent::OnEndPlay()
{
}
