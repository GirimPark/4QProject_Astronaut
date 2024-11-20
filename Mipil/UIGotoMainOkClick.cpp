#include "pch.h"
#include "UIGotoMainOkClick.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"

void UIGotoMainOkClick::Execute(UIObject& onwer, float deltaTime)
{
	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::MAIN);
}

void UIGotoMainOkClick::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}
