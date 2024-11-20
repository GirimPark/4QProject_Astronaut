#include "pch.h"
#include "OnGridCollisionHandler.h"

#include "PuzzleWorld.h"
#include "GridMovementComponent.h"

#include "../Engine/GameObject.h"
#include "../Engine/PhysicsManager.h"
#include "../Engine/EventManager.h"
#include "PlayerObject.h"

void OnGridCollisionHandler::OnContactEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
	if (actor1.lock()->GetObjectType() == eObjectType::PLAYER)
	{
		auto playerMoveCp = actor1.lock()->GetComponentByTypeName<GridMovementComponent>().lock();
		auto player = std::dynamic_pointer_cast<PlayerObject>(actor1.lock());

		//if (player->GetTurn())
		{
			if (actor2.lock()->GetObjectType() == eObjectType::LEVEL ||
				actor2.lock()->GetObjectType() == eObjectType::PLAYER ||
				actor2.lock()->GetObjectType() == eObjectType::HOLLOWBOX)
			{
				playerMoveCp->SetIsMove(false);
				playerMoveCp->SetIsCrash(true);

				std::vector<PxContactPairPoint> contactPoints;
				contactPoints.resize(cp.contactCount);
				cp.extractContacts(&contactPoints[0], cp.contactCount);

				for (PxU32 j = 0; j < cp.contactCount; j++)
				{
					EventManager::GetInstance()->SendEvent(eEventType::PLAY_CRASH_PARTICLE, nullptr, Math::Vector3(contactPoints[j].position.x, contactPoints[j].position.y, contactPoints[j].position.z));
				}

				EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_CRASH, nullptr, true);
			}
		}
	}
}

void OnGridCollisionHandler::OnContactStay(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void OnGridCollisionHandler::OnContactExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void OnGridCollisionHandler::OnTriggerEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp)
{
	if (actor1.lock()->GetObjectType() == eObjectType::PLAYER &&
		actor2.lock()->GetObjectType() == eObjectType::PART)
	{
		// actor1 -> 플레이어 오브젝트로 다이나믹캐스팅하고, 먹은 파츠 개수 올려주기
		auto world = std::dynamic_pointer_cast<PuzzleWorld>(actor1.lock()->GetOwnerWorld().lock());
		world->SubNumParts();

		EventManager::GetInstance()->SendEvent(eEventType::DELETE_OBJECT, nullptr, (actor2.lock()->GetName()).c_str());

		EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_GET_ITEM, nullptr, true);
	}
}

void OnGridCollisionHandler::OnTriggerExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp)
{
}
