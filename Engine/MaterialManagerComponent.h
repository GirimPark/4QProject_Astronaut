#pragma once
#include "Component.h"

#include "../D3DRenderer/Material.h"

class SkeletalMeshComponent;
class UIMeshComponent;
class StaticMeshComponent;

class MaterialManagerComponent : public Component
{
public:
	MaterialManagerComponent() = default;
	~MaterialManagerComponent() override;

public:
	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void ChangedMaterial(std::weak_ptr<SkeletalMeshComponent> meshComponent, const std::string& changedMaterialName, int instanceNumber, bool isComeback = false, float comebackTime = 1.f);
	void ChangedMaterial(std::weak_ptr<UIMeshComponent> meshComponent, const std::string& changedMaterialName, int instanceNumber, bool isComeback = false, float comebackTime = 1.f);
	void MaterialSetting(const std::string& materialName, const std::wstring& texturePath, MaterialMapFlags materialTextureType);

private:
	std::weak_ptr<SkeletalMeshComponent> m_SkeletalMeshComponent;
	std::weak_ptr<UIMeshComponent> m_UIMeshComponent;

	Material* m_PreviousMaterial;
	Material* m_CurrentMaterial;

	int		m_MeshInstanceNumber;
	bool	m_bIsComeBack;
	float	m_ComebackTime;
	float	m_DurationTime;
};