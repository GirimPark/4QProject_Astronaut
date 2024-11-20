#include "pch.h"
#include "ClearCollisionHandler.h"
#include "../Engine/EventManager.h"
#include "../Engine/GameObject.h"
#include "../Engine/WorldManager.h"
#include "EndingFreeWorld.h"
#include "StartFreeWorld.h"

void ClearCollisionHandler::OnContactEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void ClearCollisionHandler::OnContactStay(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void ClearCollisionHandler::OnContactExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void ClearCollisionHandler::OnTriggerEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp)
{
	if (actor1.lock()->GetObjectType() == eObjectType::READYTRIGGER &&
		actor2.lock()->GetObjectType() == eObjectType::PLAYER)
	{
		auto world = actor1.lock()->GetOwnerWorld().lock();
		assert(world);

		auto endworld = std::dynamic_pointer_cast<EndingFreeWorld>(world);

		if (endworld)
		{
			endworld->TriggerDim(true);
			endworld->DeletePlayerController();

			EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::ENDCUTSCENE);
		}
	}
}

void ClearCollisionHandler::OnTriggerExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp)
{
}

void ClearCollisionHandler::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}
