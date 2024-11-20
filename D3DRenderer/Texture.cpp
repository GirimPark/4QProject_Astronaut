#include "pch.h"
#include "Texture.h"

#include "D3DRenderManager.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Create(const std::wstring& filePath)
{
	HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice.Get(), filePath.c_str(), m_pTextureSRV.GetAddressOf()));
	m_FilePath = filePath;
}