#include "pch.h"
#include "RigidBodyComponent.h"
#include "PhysicsManager.h"
#include "SceneComponent.h"
#include "GameObject.h"

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::Initialize()
{
}

void RigidBodyComponent::Update(float deltaTime)
{
}

PxMaterial* RigidBodyComponent::CreateMaterial(float staticFriction, float dynamicFriction, float restitution)
{
	return PhysicsManager::GetInstance()->GetPhysics()->createMaterial(staticFriction, dynamicFriction, restitution);
}

void RigidBodyComponent::SetCollisionFilter(PxRigidActor* actor)
{
	PxFilterData filterData;

	filterData.word0 = 1 << static_cast<PxU32>(m_pOwner.lock()->GetObjectType());

	PxShape* shapes[10];
	actor->getShapes(shapes, 10);

	for (PxU32 i = 0; i < actor->getNbShapes(); ++i)
	{
		shapes[i]->setSimulationFilterData(filterData);
		shapes[i]->setQueryFilterData(filterData);
	}
}

PxTransform RigidBodyComponent::ConvertPxTransform()
{
	Math::Quaternion ownQuat;
	auto& ownerPosition = m_pOwner.lock()->GetLocalPosition();
	auto& ownerRotation = m_pOwner.lock()->GetLocalRotation();
	ownQuat = Math::Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(ownerRotation.y), XMConvertToRadians(ownerRotation.x), XMConvertToRadians(ownerRotation.z));

	return PxTransform(PxVec3(ownerPosition.x, ownerPosition.y, ownerPosition.z), PxQuat(ownQuat.x, ownQuat.y, ownQuat.z, ownQuat.w));
}
