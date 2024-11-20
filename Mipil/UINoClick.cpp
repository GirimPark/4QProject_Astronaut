#include "pch.h"
#include "UINoClick.h"
#include "../Engine/UIObject.h"

void UINoClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	onwer.GetParent().lock()->SetAllVisible(false);
}

void UINoClick::HandleEvent(Event* event)
{
}
