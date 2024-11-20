#include "pch.h"
#include "PlayerFreeObject.h"
#include "PlayerMovementContorller.h"

#include "../Engine/SkeletalMeshComponent.h"
#include "../Engine/RigidDynamicComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/WorldManager.h"

#include "FireParticleObject.h"
#include "PointLightComponent.h"


PlayerFreeObject::PlayerFreeObject()
{
}

PlayerFreeObject::~PlayerFreeObject()
{
}

void PlayerFreeObject::Fire(bool isPlay)
	{
	m_fireParticleObject->PlayParticle(isPlay);
}

void PlayerFreeObject::Initialize()
{
	auto meshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent").lock();

	SetRootComponent(meshComponent);
	if (WorldManager::GetInstance()->IsHostServer())
	{
		// bear
		meshComponent->SetDefaultAnimation("../Resources/FBX/bear_Idle.fbx");
		meshComponent->AddAnimation("../Resources/FBX/bear_move.fbx", true);
	}
	else
	{
		// rabbit
		meshComponent->SetDefaultAnimation("../Resources/FBX/rabbit_idle_.fbx");
		meshComponent->AddAnimation("../Resources/FBX/rabbit_move_.fbx", true);
	}

	SetPosition(m_deferredPosition);
	SetRotation(m_deferredRotation);
	SetScale({ 0.5f, 0.5f, 0.5f });
	meshComponent->SetOffsetPosition({ 0.f, -67.f, 0.f });

	auto rigidDynamicComponent = CreateComponent<RigidDynamicComponent>("Capsule_Rigidbody").lock();
	rigidDynamicComponent->CreateDynamicRigidBody(RigidBodyComponent::Geometry::Capsule, { 61.25f, 21.f }, { 0.5f, 0.5f, 0.0f }, 0.01f);
	rigidDynamicComponent->SetLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | PxRigidDynamicLockFlag::eLOCK_LINEAR_Y);
	//rigidDynamicComponent->AddCollisionHandler<OnGridCollisionHandler>();
	auto controllerComponent = CreateComponent<ControllerComponent>("Player_Controller_Component");
	controllerComponent.lock()->CreateController<PlayerMovementContorller>();

	GameObject::Initialize();
}

void PlayerFreeObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
