#include "pch.h"
#include "RenderComponent.h"

void RenderComponent::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_boundingBox.Center = GetWorldPosition() + m_boundingBoxOffset;
}
