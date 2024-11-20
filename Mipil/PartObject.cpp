#include "pch.h"
#include "PartObject.h"

#include "PartPOD.h"

#include "../Engine/StaticMeshComponent.h"
#include "../Engine/RigidStaticComponent.h"

void PartObject::Initialize()
{
	std::shared_ptr<StaticMeshComponent> meshComponent = CreateComponent<StaticMeshComponent>("meshComponent").lock();
	meshComponent->SetFilePath(m_filePath);
	SetRootComponent(meshComponent);
	meshComponent->SetLocalPosition(m_partPOD->position);
	meshComponent->SetLocalRotation({ -XMConvertToDegrees(m_partPOD->rotation.ToEuler().x), -XMConvertToDegrees(m_partPOD->rotation.ToEuler().y), -XMConvertToDegrees(m_partPOD->rotation.ToEuler().z) });
	meshComponent->SetLocalScale(m_partPOD->scale);

	auto BoxRigidBody = CreateComponent<RigidStaticComponent>("Part_Rigidbody").lock();
	BoxRigidBody->CreateStaticRigidBody(RigidBodyComponent::Geometry::Box, { 50.f, 50.f, 50.f }, { 0.5f, 0.5f, 0.0f }, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, true);
	GameObject::Initialize();

	meshComponent->SetOutLine(true);
}

void PartObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
