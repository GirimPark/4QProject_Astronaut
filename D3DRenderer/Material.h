#pragma once

struct aiMaterial;


enum MaterialMapFlags
{
	BASECOLOR = 1 << 0,
	NORMAL = 1 << 1,
	SPECULAR = 1 << 2,
	EMISSIVE = 1 << 3,
	OPACITY = 1 << 4,
	METALNESS = 1 << 5,
	ROUGHNESS = 1 << 6
};


class MaterialTexture
{
public:
	MaterialTexture();
	~MaterialTexture();

	std::wstring m_FilePath;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV = nullptr;	// ≈ÿΩ∫√≥ ∏Æº“Ω∫ ∫‰.
	void Create(const std::wstring& filePath);
};


class Material
{
public:
	Material();
	~Material();

	void Create(aiMaterial* pMaterial);
	void SetTexture(MaterialMapFlags materialFlag, std::wstring texturePath);

	Vector4 m_Color;

	std::shared_ptr<MaterialTexture> m_pDiffuseRV = nullptr;	// ≈ÿΩ∫√≥ ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<MaterialTexture> m_pNormalRV = nullptr;	    // ≥Î∏ª∏  ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<MaterialTexture> m_pSpecularRV = nullptr;	// Ω∫∆‰≈ß∑Ø∏  ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<MaterialTexture> m_pEmissiveRV = nullptr;	// ¿ÃπÃΩ√∫Í∏  ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<MaterialTexture> m_pOpacityRV = nullptr;	// ≈ı∏Ì∏  ∏Æº“Ω∫ ∫‰.	
	std::shared_ptr<MaterialTexture> m_pMetalnessRV = nullptr;	// ∏ﬁ≈ª∏  ∏Æº“Ω∫ ∫‰.	
	std::shared_ptr<MaterialTexture> m_pRoughnessRV = nullptr;	// ∑Ø«¡¥œΩ∫∏  ∏Æº“Ω∫ ∫‰.	

	UINT m_MaterialMapFlags = 0;
};
	