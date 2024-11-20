#include "pch.h"
#include "MaterialManagerComponent.h"

#include "SkeletalMeshComponent.h"
#include "UIMeshComponent.h"

#include "../D3DRenderer/SkeletalMeshModel.h"
#include "../D3DRenderer/ResourceManager.h"

using namespace std;

MaterialManagerComponent::~MaterialManagerComponent()
{
}

void MaterialManagerComponent::Initialize()
{
	m_MeshInstanceNumber = 0;
	m_bIsComeBack = false;
	m_ComebackTime = 0.f;
	m_DurationTime = 0.f;
}

void MaterialManagerComponent::Update(float deltaTime)
{
	m_DurationTime += deltaTime;

	// Material이 Comeback해야 한다면
	// 입력한 시간이 지나면 Previous Material으로 다시 세팅하기
	if (m_bIsComeBack && m_ComebackTime <= m_DurationTime)
	{
		if (m_SkeletalMeshComponent.lock())
		{
			m_SkeletalMeshComponent.lock()->SetMaterial(m_PreviousMaterial, m_MeshInstanceNumber);
			m_SkeletalMeshComponent.lock() = nullptr;
		}

		if (m_UIMeshComponent.lock())
		{
			m_UIMeshComponent.lock()->SetMaterial(m_PreviousMaterial, m_MeshInstanceNumber);
			m_UIMeshComponent.lock() = nullptr;
		}
	}
}


/// 3D UI Mesh의 Material 변경하기 ///
void MaterialManagerComponent::ChangedMaterial(std::weak_ptr<SkeletalMeshComponent> meshComponent, const std::string& changedMaterialName, int instanceNumber, bool isComeback, float comebackTime)
{
	m_MeshInstanceNumber = instanceNumber;
	m_bIsComeBack = isComeback;
	m_ComebackTime = comebackTime;
	m_DurationTime = 0.f;

	m_SkeletalMeshComponent = meshComponent;
	m_CurrentMaterial = ResourceManager::GetInstance()->CreateMaterial(changedMaterialName).get();

	if (m_PreviousMaterial == nullptr)
		m_PreviousMaterial = meshComponent.lock()->GetSkeletalMeshModel()->GetMaterial(instanceNumber);

	// 해당 메시의 컴포넌트에 머터리얼을 세팅하면 이전의 머터리얼을 반환한다.
	m_SkeletalMeshComponent.lock()->SetMaterial(m_CurrentMaterial, m_MeshInstanceNumber);
}


/// 3D UI Mesh의 Material 변경하기 ///
void MaterialManagerComponent::ChangedMaterial(std::weak_ptr<UIMeshComponent> meshComponent, const std::string& changedMaterialName, int instanceNumber, bool isComeback, float comebackTime)
{
	m_MeshInstanceNumber = instanceNumber;
	m_bIsComeBack = isComeback;
	m_ComebackTime = comebackTime;
	m_DurationTime = 0.f;

	m_UIMeshComponent = meshComponent;

	if (m_PreviousMaterial == nullptr)
		m_PreviousMaterial = meshComponent.lock()->GetSkeletalMeshModel()->GetMaterial(instanceNumber);

	m_CurrentMaterial = ResourceManager::GetInstance()->CreateMaterial(changedMaterialName).get();

	// 해당 메시의 컴포넌트에 머터리얼을 세팅하면 이전의 머터리얼을 반환한다.
	m_UIMeshComponent.lock()->SetMaterial(m_CurrentMaterial, m_MeshInstanceNumber);
}


/// Material 세팅하기 ( Material 생성하고 해당 Material에 텍스쳐 세팅 ) ///
void MaterialManagerComponent::MaterialSetting(const std::string& materialName, const std::wstring& texturePath, MaterialMapFlags materialTextureType)
{
	shared_ptr<Material> material = ResourceManager::GetInstance()->CreateMaterial(materialName);
	material->SetTexture(materialTextureType, texturePath);

}
