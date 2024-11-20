#include "pch.h"
#include "ReadyCollisionHandler.h"
#include "../Engine/GameObject.h"
#include "../Engine/WorldManager.h"

#include "EndingFreeWorld.h"
#include "StartFreeWorld.h"

void ReadyCollisionHandler::OnContactEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void ReadyCollisionHandler::OnContactStay(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void ReadyCollisionHandler::OnContactExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp)
{
}

void ReadyCollisionHandler::OnTriggerEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp)
{
	if (actor1.lock()->GetObjectType() == eObjectType::READYTRIGGER &&
		actor2.lock()->GetObjectType() == eObjectType::PLAYER)
	{
		auto world = actor1.lock()->GetOwnerWorld().lock();
		assert(world);

		auto startworld = std::dynamic_pointer_cast<StartFreeWorld>(world);

		if (startworld)
		{
			startworld->TriggerDim(true);
			startworld->DeletePlayerController();

			WorldManager::GetInstance()->PushSendQueue(
				WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_IsAllGoal), C2S_IS_ALL_GOAL, nullptr),
				sizeof(PacketC2S_IsAllGoal));
		}
	}
}

void ReadyCollisionHandler::OnTriggerExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp)
{
}
