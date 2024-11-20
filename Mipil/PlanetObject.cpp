#include "pch.h"
#include "PlanetObject.h"

#include "../Engine/StaticMeshComponent.h"


void PlanetObject::Initialize()
{
	m_Rotation = 0.f;

	m_pMeshComponent = CreateComponent<StaticMeshComponent>("MeshComponent").lock();

	//m_pMeshComponent->SetFilePath("../Resources/FBX/earth.fbx");
	m_pMeshComponent->SetFilePath(m_Path);
	m_pMeshComponent->SetVisible(true);
	SetRootComponent(m_pMeshComponent);

	GameObject::Initialize();
}

void PlanetObject::Update(float deltaTime)
{
	m_Rotation += deltaTime * 5.f;
	m_pMeshComponent->SetLocalRotation(Vector3(0.f, m_Rotation, 0.f));

	GameObject::Update(deltaTime);
}
