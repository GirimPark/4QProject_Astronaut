#include "pch.h"
#include "CollisionEventCallback.h"
#include "PhysicsManager.h"
#include "CollisionHandler.h"

void CollisionEventCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void CollisionEventCallback::onWake(PxActor** actors, PxU32 count)
{
}

void CollisionEventCallback::onSleep(PxActor** actors, PxU32 count)
{
}

void CollisionEventCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}

void CollisionEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; i++) {
		const physx::PxContactPair& cp = pairs[i];
		physx::PxActor* actor1 = pairHeader.actors[0];
		physx::PxActor* actor2 = pairHeader.actors[1];

		auto collisionHandler1 = PhysicsManager::GetInstance()->GetCollisionHandler(actor1);
		auto collisionHandler2 = PhysicsManager::GetInstance()->GetCollisionHandler(actor2);

		auto collisionOwner1 = PhysicsManager::GetInstance()->GetCollisionOwner(actor1);
		auto collisionOwner2 = PhysicsManager::GetInstance()->GetCollisionOwner(actor2);

		if (!collisionHandler1.expired()) {
			HandleContactEvent(collisionHandler1.lock(), collisionOwner1, collisionOwner2, cp);
		}

		if (!collisionHandler2.expired()) {
			HandleContactEvent(collisionHandler2.lock(), collisionOwner2, collisionOwner1, cp);
		}
	}
}

void CollisionEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++) {
		const physx::PxTriggerPair& tp = pairs[i];
		physx::PxActor* triggerActor = tp.triggerActor;
		physx::PxActor* otherActor = tp.otherActor;

		auto triggerHandler = PhysicsManager::GetInstance()->GetCollisionHandler(triggerActor);
		auto otherHandler = PhysicsManager::GetInstance()->GetCollisionHandler(otherActor);

		auto triggerHandlerOnwer = PhysicsManager::GetInstance()->GetCollisionOwner(triggerActor);
		auto otherHandlerOnwer = PhysicsManager::GetInstance()->GetCollisionOwner(otherActor);

		if (!triggerHandler.expired()) {
			HandleTriggerEvent(triggerHandler.lock(), triggerHandlerOnwer, otherHandlerOnwer, tp);
		}

		if (!otherHandler.expired()) {
			HandleTriggerEvent(otherHandler.lock(), otherHandlerOnwer, triggerHandlerOnwer, tp);
		}
	}
}



void CollisionEventCallback::HandleContactEvent(std::shared_ptr<CollisionHandler> handler, std::weak_ptr<GameObject> actor, std::weak_ptr<GameObject> otherActor, const physx::PxContactPair& cp) {
	if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
		handler->OnContactEnter(actor, otherActor, cp);
	}
	else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
		handler->OnContactStay(actor, otherActor, cp);
	}
	else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) {
		handler->OnContactExit(actor, otherActor, cp);
	}
}

void CollisionEventCallback::HandleTriggerEvent(std::shared_ptr<CollisionHandler> handler, std::weak_ptr<GameObject> triggerActor, std::weak_ptr<GameObject> otherActor, const physx::PxTriggerPair& tp) {
	if (tp.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
		handler->OnTriggerEnter(triggerActor, otherActor, tp);
	}
	else if (tp.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) {
		handler->OnTriggerExit(triggerActor, otherActor, tp);
	}
}