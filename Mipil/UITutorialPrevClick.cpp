#include "pch.h"
#include "UITutorialPrevClick.h"
#include "../Engine/UIObject.h"

void UITutorialPrevClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	(*m_Count)--;
}
