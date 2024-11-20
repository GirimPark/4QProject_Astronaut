#include "pch.h"
#include "UIRestartClickFunctor.h"
#include "GameOverWorld.h"

#include "../Engine/WorldManager.h"
#include "../Engine/UIPanel.h"
#include "../NetworkLibrary/MyProtocol.h"

UIRestartClickFunctor::~UIRestartClickFunctor()
{
}

void UIRestartClickFunctor::Execute(UIObject& onwer, float deltaTime)
{
	PacketC2S_RestartGame rg;
	rg.restart = '1';

	WorldManager::GetInstance()->PushSendQueue(
		WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_RestartGame), C2S_RESTART_GAME, &rg.restart), sizeof(PacketC2S_RestartGame));

	auto loagingUI = WorldManager::GetInstance()->GetCurWorld()->GetUIManager()->GetUIObject<UIPanel>(L"LoadingPanelUI");
	loagingUI.lock()->SetAllVisible(true);	

	std::shared_ptr<GameOverWorld> pGameOverWorld = std::dynamic_pointer_cast<GameOverWorld>(WorldManager::GetInstance()->GetCurWorld());
	pGameOverWorld->SetLoading(true);
}
