#include "pch.h"
#include "EnvironmentComponent.h"

#include "CommonApp.h"
#include "../D3DRenderer/EnvironmentModel.h"

EnvironmentComponent::~EnvironmentComponent()
{
	m_environmentModel.reset();
}

void EnvironmentComponent::Initialize()
{
	assert(!m_filePath.empty());
	assert(!m_environmentTexturePath.empty());
	assert(!m_IBLSpecularTexturePath.empty());
	assert(!m_IBLDiffuseTexturePath.empty());
	assert(!m_IBLBRDFTexturePath.empty());

	m_environmentModel = std::make_shared<EnvironmentModel>();
	m_environmentModel->SetWorldMatrix(&m_worldMatrix);

	m_environmentModel->ReadEnvironmentMeshFromFBX(m_filePath);
	m_environmentModel->ReadEnvironmentTextureFromDDS(m_environmentTexturePath);
	m_environmentModel->ReadIBLDiffuseTextureFromDDS(m_IBLDiffuseTexturePath);
	m_environmentModel->ReadIBLSpecularTextureFromDDS(m_IBLSpecularTexturePath);
	m_environmentModel->ReadIBLBRDFTextureFromDDS(m_IBLBRDFTexturePath);

	CommonApp::m_pInstance->GetRenderer()->SetEnvironmentModel(m_environmentModel);

	RenderComponent::Initialize();
}

void EnvironmentComponent::Update(float deltaTime)
{
	RenderComponent::Update(deltaTime);
}
