#include "pch.h"
#include "UITimerFunctor.h"

void UITimerFunctor::SetTargetUI(std::weak_ptr<UIObject> targetUI)
{
	m_targetUI = targetUI;
}