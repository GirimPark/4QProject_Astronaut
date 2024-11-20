#include "pch.h"
#include "PointLightComponent.h"

#include "../Engine/GameObject.h"
#include "../Engine/CommonApp.h"

PointLightComponent::PointLightComponent()
{
	m_pointLight = std::make_shared<PointLightInstance>();
}

PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::Initialize()
{
}

void PointLightComponent::Update(float deltaTime)
{
	auto& position = m_pOwner.lock()->GetWorldPosition();
	m_pointLight->m_PointLightPos.x = position.x;
	m_pointLight->m_PointLightPos.y = position.y;
	m_pointLight->m_PointLightPos.z = position.z;

	CommonApp::m_pInstance->GetRenderer()->AddPointLightInstance(m_pointLight);
}

void PointLightComponent::SetLightColor(float _r, float _g, float _b)
{
	m_pointLight->m_PointLightColor.x = _r;
	m_pointLight->m_PointLightColor.y = _g;
	m_pointLight->m_PointLightColor.z = _b;
}
