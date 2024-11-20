#include "pch.h"
#include "UITutorialExitClick.h"
#include "PuzzleWorld.h"
#include "../Engine/UIPanel.h"
#include "../Engine/WorldManager.h"

void UITutorialExitClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	*m_bStage1Guide = false;

	onwer.GetParent().lock()->SetAllVisible(false);

	auto pPuzzleWorld = std::dynamic_pointer_cast<PuzzleWorld>(WorldManager::GetInstance()->GetCurWorld());
	pPuzzleWorld->UIChange();

	WorldManager::GetInstance()->PushSendQueue(
		WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_IsAllEnter), C2S_IS_ALL_ENTER, nullptr),
		sizeof(PacketC2S_IsAllEnter));
}