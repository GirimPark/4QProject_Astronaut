#pragma once

#include "StaticMeshInstance.h"
#include "RenderTransform.h"

class MaterialTexture;
class ModelResource;

class EnvironmentMeshComponent : public RenderTransform
{
public:
	EnvironmentMeshComponent();
	~EnvironmentMeshComponent() = default;

	std::shared_ptr<ModelResource>		m_SceneResource;
	std::shared_ptr<MaterialTexture>	m_EnvironmentTextureResource;
	std::shared_ptr<MaterialTexture>	m_IBLDiffuseTextureResource;
	std::shared_ptr<MaterialTexture>	m_IBLSpecularTextureResource;
	std::shared_ptr<MaterialTexture>	m_IBLBRDFTextureResource;
	StaticMeshInstance					m_MeshInstance;

	std::string m_SceneFilePath; // BeginPlay에서 로딩
	std::wstring m_EnvironmentTexturePath; // BeginPlay에서 로딩
	std::wstring m_IBLSpecularTexturePath; // BeginPlay에서 로딩
	std::wstring m_IBLDiffuseTexturePath; // BeginPlay에서 로딩
	std::wstring m_IBLBRDFTexturePath; // BeginPlay에서 로딩

	bool ReadEnvironmentMeshFromFBX(std::string filePath);
	bool ReadEnvironmentTextureFromDDS(std::wstring filePath);
	bool ReadIBLDiffuseTextureFromDDS(std::wstring filePath);
	bool ReadIBLSpecularTextureFromDDS(std::wstring filePath);
	bool ReadIBLBRDFTextureFromDDS(std::wstring filePath);
};

