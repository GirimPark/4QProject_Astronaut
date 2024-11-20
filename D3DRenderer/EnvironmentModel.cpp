#include "pch.h"

#include "EnvironmentModel.h"
#include "ResourceManager.h"

bool EnvironmentModel::ReadEnvironmentMeshFromFBX(std::string_view filePath)
{
	// FBX 파일 읽기	
	m_SceneResource = ResourceManager::GetInstance()->CreateModelResource(std::string(filePath), ModelType::STATIC);

	assert(m_SceneResource != nullptr);
	if (m_SceneResource == nullptr)
	{
		return false;
	}
	m_MeshInstance.Create(&m_SceneResource->m_Meshes[0], m_worldMatrix, nullptr, &m_SceneResource->m_InstanceBoundingBox[0]);
	m_SceneFilePath = filePath;
	return true;
}

bool EnvironmentModel::ReadEnvironmentTextureFromDDS(std::wstring filePath)
{
	m_EnvironmentTextureResource = ResourceManager::GetInstance()->CreateMaterialTexture(filePath);
	assert(m_EnvironmentTextureResource != nullptr);
	if (m_EnvironmentTextureResource == nullptr)
	{
		return false;
	}
	m_EnvironmentTexturePath = filePath;
	return true;
}

bool EnvironmentModel::ReadIBLDiffuseTextureFromDDS(std::wstring filePath)
{
	m_IBLDiffuseTextureResource = ResourceManager::GetInstance()->CreateMaterialTexture(filePath);
	assert(m_IBLDiffuseTextureResource != nullptr);
	if (m_IBLDiffuseTextureResource == nullptr)
	{
		return false;
	}
	m_IBLDiffuseTexturePath = filePath;
	return true;
}

bool EnvironmentModel::ReadIBLSpecularTextureFromDDS(std::wstring filePath)
{
	m_IBLSpecularTextureResource = ResourceManager::GetInstance()->CreateMaterialTexture(filePath);
	assert(m_IBLSpecularTextureResource != nullptr);
	if (m_IBLSpecularTextureResource == nullptr)
	{
		return false;
	}
	m_IBLSpecularTexturePath = filePath;
	return true;
}

bool EnvironmentModel::ReadIBLBRDFTextureFromDDS(std::wstring filePath)
{
	m_IBLBRDFTextureResource = ResourceManager::GetInstance()->CreateMaterialTexture(filePath);
	assert(m_IBLBRDFTextureResource != nullptr);
	if (m_IBLBRDFTextureResource == nullptr)
	{
		return false;
	}
	m_IBLBRDFTexturePath = filePath;
	return true;
}
