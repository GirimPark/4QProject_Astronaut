#include "pch.h"
#include "UIEndGameOkClick.h"

#include "../Engine/CommonApp.h"

void UIEndGameOkClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	CommonApp::m_pInstance->Exit();
}
