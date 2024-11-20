#include "pch.h"
#include "Material.h"
#include "D3DRenderManager.h"
#include "ResourceManager.h"

Material::Material()
{

}

Material::~Material()
{
	m_pDiffuseRV.reset();
	m_pNormalRV.reset();
	m_pSpecularRV.reset();
	m_pEmissiveRV.reset();
	m_pOpacityRV.reset();
	m_pMetalnessRV.reset();
	m_pRoughnessRV.reset();
}

void Material::Create(aiMaterial* pMaterial)
{
	aiString texturePath;
	wstring basePath = L"../Resources/Textures/";
	std::filesystem::path path;
	wstring finalPath;
	string name = pMaterial->GetName().C_Str();

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pDiffuseRV = ResourceManager::GetInstance()->CreateMaterialTexture(finalPath);
		m_MaterialMapFlags |= MaterialMapFlags::BASECOLOR;
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pNormalRV = ResourceManager::GetInstance()->CreateMaterialTexture(finalPath);
		m_MaterialMapFlags |= MaterialMapFlags::NORMAL;
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pSpecularRV = ResourceManager::GetInstance()->CreateMaterialTexture(finalPath);
		m_MaterialMapFlags |= MaterialMapFlags::SPECULAR;
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pEmissiveRV = ResourceManager::GetInstance()->CreateMaterialTexture(finalPath);
		m_MaterialMapFlags |= MaterialMapFlags::EMISSIVE;
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pOpacityRV = ResourceManager::GetInstance()->CreateMaterialTexture(finalPath);
		m_MaterialMapFlags |= MaterialMapFlags::OPACITY;
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_METALNESS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pMetalnessRV = ResourceManager::GetInstance()->CreateMaterialTexture(finalPath);
		m_MaterialMapFlags |= MaterialMapFlags::METALNESS;
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_SHININESS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_pRoughnessRV = ResourceManager::GetInstance()->CreateMaterialTexture(finalPath);
		m_MaterialMapFlags |= MaterialMapFlags::ROUGHNESS;
	}
}

void Material::SetTexture(MaterialMapFlags materialFlag, std::wstring texturePath)
{
	if (materialFlag & MaterialMapFlags::BASECOLOR)
	{
		m_pDiffuseRV = ResourceManager::GetInstance()->CreateMaterialTexture(texturePath);

		if (!(m_MaterialMapFlags & MaterialMapFlags::BASECOLOR))
			m_MaterialMapFlags |= MaterialMapFlags::BASECOLOR;
	}
	else if (materialFlag & MaterialMapFlags::NORMAL)
	{
		m_pNormalRV = ResourceManager::GetInstance()->CreateMaterialTexture(texturePath);

		if (!(m_MaterialMapFlags & MaterialMapFlags::NORMAL))
			m_MaterialMapFlags |= MaterialMapFlags::NORMAL;
	}
	else if (materialFlag & MaterialMapFlags::SPECULAR)
	{
		m_pSpecularRV = ResourceManager::GetInstance()->CreateMaterialTexture(texturePath);

		if (!(m_MaterialMapFlags & MaterialMapFlags::SPECULAR))
			m_MaterialMapFlags |= MaterialMapFlags::SPECULAR;
	}
	else if (materialFlag & MaterialMapFlags::EMISSIVE)
	{
		m_pEmissiveRV = ResourceManager::GetInstance()->CreateMaterialTexture(texturePath);

		if (!(m_MaterialMapFlags & MaterialMapFlags::EMISSIVE))
			m_MaterialMapFlags |= MaterialMapFlags::EMISSIVE;
	}
	else if (materialFlag & MaterialMapFlags::OPACITY)
	{
		m_pOpacityRV = ResourceManager::GetInstance()->CreateMaterialTexture(texturePath);

		if (!(m_MaterialMapFlags & MaterialMapFlags::OPACITY))
			m_MaterialMapFlags |= MaterialMapFlags::OPACITY;
	}
	else if (materialFlag & MaterialMapFlags::METALNESS)
	{
		m_pMetalnessRV = ResourceManager::GetInstance()->CreateMaterialTexture(texturePath);

		if (!(m_MaterialMapFlags & MaterialMapFlags::METALNESS))
			m_MaterialMapFlags |= MaterialMapFlags::METALNESS;
	}
	else if (materialFlag & MaterialMapFlags::ROUGHNESS)
	{
		m_pRoughnessRV = ResourceManager::GetInstance()->CreateMaterialTexture(texturePath);

		if (!(m_MaterialMapFlags & MaterialMapFlags::ROUGHNESS))
			m_MaterialMapFlags |= MaterialMapFlags::ROUGHNESS;
	}
}

MaterialTexture::MaterialTexture()
{
}

MaterialTexture::~MaterialTexture()
{
	m_pTextureSRV.Reset();
}

void MaterialTexture::Create(const std::wstring& filePath)
{
	HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice.Get(), filePath.c_str(), m_pTextureSRV.GetAddressOf()));
	m_FilePath = filePath;
}
