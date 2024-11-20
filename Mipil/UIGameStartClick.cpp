#include "pch.h"
#include "UIGameStartClick.h"
#include "../Engine/EventManager.h"
#include "../Engine/World.h"
#include "../Engine/WorldManager.h"

void UIGameStartClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	//LOG_ENGINE("GameStart UI Click");
	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STARTCUTSCECNE);
}

void UIGameStartClick::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}
