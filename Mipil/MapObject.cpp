#include "pch.h"
#include "MapObject.h"
#include "../Engine/StaticMeshComponent.h"
#include "../Engine/RigidStaticComponent.h"

void MapObject::Initialize()
{
	std::shared_ptr<StaticMeshComponent> meshComponent = CreateComponent<StaticMeshComponent>("Root_Component").lock();
	meshComponent->SetFilePath(m_filePath);
	SetRootComponent(meshComponent);
	meshComponent->Initialize();

	auto BoxRigidBody = CreateComponent<RigidStaticComponent>("Map_Rigidbody").lock();
	BoxRigidBody->CreateStaticRigidBodyFromStaticMesh(meshComponent.get(), { 0.5f, 0.5f, 0.0f });
	BoxRigidBody->Initialize();
	//GameObject::Initialize();
}

void MapObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
