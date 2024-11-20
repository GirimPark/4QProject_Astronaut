#include "pch.h"
#include "TestCollisionHandler.h"
#include "../Engine/GameObject.h"
#include "../Engine/RigidDynamicComponent.h"

void TestCollisionHandler::OnContactEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
	if (actor1.lock()->GetObjectType() == eObjectType::PLAYER)
	{
		if (actor2.lock()->GetObjectType() == eObjectType::LEVEL ||
			actor2.lock()->GetObjectType() == eObjectType::PLAYER)
		{
			auto playerMoveCp = actor1.lock()->GetComponentByTypeName<RigidDynamicComponent>().lock();
			playerMoveCp->GetRigidDynamic()->addForce(physx::PxVec3(-40000.f, 0, 0), PxForceMode::eIMPULSE);
		}
	}

}

void TestCollisionHandler::OnContactStay(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void TestCollisionHandler::OnContactExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void TestCollisionHandler::OnTriggerEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp)
{
}

void TestCollisionHandler::OnTriggerExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp)
{
}
