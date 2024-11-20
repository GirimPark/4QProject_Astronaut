#include "pch.h"
#include "UITutorialNextClick.h"
#include "../Engine/UIObject.h"

void UITutorialNextClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);
	
	(*m_Count)++;
}
