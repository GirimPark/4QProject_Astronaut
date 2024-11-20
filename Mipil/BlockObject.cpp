#include "pch.h"
#include "BlockObject.h"

#include "BlockPOD.h"

#include "../Engine/StaticMeshComponent.h"
#include "../Engine/RigidStaticComponent.h"

void BlockObject::Initialize()
{
	std::shared_ptr<StaticMeshComponent> meshComponent = CreateComponent<StaticMeshComponent>("meshComponent").lock();
	meshComponent->SetFilePath(m_filePath);
	SetRootComponent(meshComponent);

	meshComponent->SetLocalPosition(m_blockPOD->position);
	meshComponent->SetLocalRotation({ -XMConvertToDegrees(m_blockPOD->rotation.ToEuler().x), -XMConvertToDegrees(m_blockPOD->rotation.ToEuler().y), -XMConvertToDegrees(m_blockPOD->rotation.ToEuler().z) });
	meshComponent->SetLocalScale(m_blockPOD->scale);

	auto BoxRigidBody = CreateComponent<RigidStaticComponent>("Level_Rigidbody");
	BoxRigidBody.lock()->CreateStaticRigidBody(RigidBodyComponent::Geometry::Box, { 90.f, 90.f, 90.f }, { 0.5f, 0.5f, 0.0f });

	GameObject::Initialize();

}

void BlockObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
