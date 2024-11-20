#include "pch.h"
#include "UICreditClick.h"
#include "../Engine/EventManager.h"
#include "../Engine/World.h"
#include "../Engine/WorldManager.h"


void UICreditClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	LOG_ENGINE("Credit UI Click");
	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::CREDIT);
}

void UICreditClick::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}
