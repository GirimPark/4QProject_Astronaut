#include "pch.h"
#include "StaticMeshComponent.h"

#include "CommonApp.h"
#include "../D3DRenderer/StaticMeshModel.h"

StaticMeshComponent::~StaticMeshComponent()
{
	m_staticMeshModel.reset();
}

void StaticMeshComponent::SetOutLine(bool isOutLine)
{
	m_staticMeshModel->SetOutLine(isOutLine);
}

void StaticMeshComponent::Initialize()
{
	//assert(!m_filePath.empty());

	m_staticMeshModel = std::make_shared<StaticMeshModel>();
	m_staticMeshModel->SetWorldMatrix(&m_worldMatrix);
	m_staticMeshModel->ReadSceneResourceFromFBX(m_filePath);

	RenderComponent::Initialize();
}

void StaticMeshComponent::Update(float deltaTime)
{
	RenderComponent::Update(deltaTime);

	if(m_bVisible)
		CommonApp::m_pInstance->GetRenderer()->AddStaticMeshModel(m_staticMeshModel);
}
