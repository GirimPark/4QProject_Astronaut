#include "pch.h"
#include "FloorBlockObject.h"

#include "BlockPOD.h"

#include "../Engine/StaticMeshComponent.h"
#include "../Engine/RigidStaticComponent.h"

void FloorBlockObject::Initialize()
{
	m_BlockMeshComponent = CreateComponent<StaticMeshComponent>("meshComponent").lock();
	m_BlockMeshComponent->SetFilePath(m_filePath);
	SetRootComponent(m_BlockMeshComponent);

	GameObject::Initialize();
}

void FloorBlockObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
