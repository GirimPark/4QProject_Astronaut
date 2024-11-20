#pragma once
#include "PxPhysicsAPI.h"
/// <summary>
/// CollisionEventCallback
/// 피직스의 이벤트 발생 시 호출 되는 함수들
/// </summary>
using namespace physx;

class CollisionHandler;
class GameObject;

class CollisionEventCallback
	: public PxSimulationEventCallback
{
public:
	// PxSimulationEventCallback을(를) 통해 상속됨
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	void onWake(PxActor** actors, PxU32 count) override;
	void onSleep(PxActor** actors, PxU32 count) override;
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

private:
	void HandleContactEvent(std::shared_ptr<CollisionHandler> handler, std::weak_ptr<GameObject> actor, std::weak_ptr<GameObject> otherActor, const physx::PxContactPair& cp);
	void HandleTriggerEvent(std::shared_ptr<CollisionHandler> handler, std::weak_ptr<GameObject> triggerActor, std::weak_ptr<GameObject> otherActor, const physx::PxTriggerPair& tp);
};

