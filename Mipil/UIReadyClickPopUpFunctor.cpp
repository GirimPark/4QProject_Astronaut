#include "pch.h"
#include "UIReadyClickPopUpFunctor.h"

#include "../Mipil/PlayerObject.h"
#include "../Engine/UIButton.h"
#include "../Engine/World.h"

void UIReadyClickPopUpFunctor::Execute(UIObject& onwer, float deltaTime)
{
	if (!m_player.lock())
		return;

	if (m_player.lock()->GetReadyState())
		return;
	if (m_player.lock()->GetLoadingIndex() == 4)
	{
		auto readyButtonUI = m_player.lock()->GetOwnerWorld().lock()->GetUIManager()->GetUIObject<UIButton>(L"InputAction_ReadyButtonUI");
		readyButtonUI.lock()->GetUIInstance().lock()->NotLoopAnimationStart();
		__super::Execute(onwer, deltaTime);
	}
}
