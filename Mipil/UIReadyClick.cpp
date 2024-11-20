#include "pch.h"
#include "UIReadyClick.h"
#include "../Engine/UIPanel.h"
#include "../Engine/WorldManager.h"

#include "../Mipil/PlayerObject.h"

void UIReadyClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	if (!m_player.lock())
		return;

	if (m_player.lock()->GetLoadingIndex() == 4)
	{
		// 준비 버튼
		PacketC2S_READY ready;
		ready.clickedReady = '1';

		WorldManager::GetInstance()->PushSendQueue(
			WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_READY), C2S_READY, &ready.clickedReady),
			sizeof(PacketC2S_READY));

		onwer.GetParent().lock()->SetAllVisible(false);

		m_player.lock()->SetLoadingActionUI();

		m_player.lock()->SetReadyState(true);
	}
}
