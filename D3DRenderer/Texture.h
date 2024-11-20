#pragma once


class Texture
{
public:
	Texture();
	~Texture();

public:
	ComPtr<ID3D11ShaderResourceView> GetTexture() { return m_pTextureSRV; }

public:
	void Create(const std::wstring& filePath);

private:
	std::wstring m_FilePath;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV = nullptr;	// 텍스처 리소스 뷰.
};