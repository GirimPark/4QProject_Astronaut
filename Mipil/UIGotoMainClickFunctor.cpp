#include "pch.h"
#include "UIGotoMainClickFunctor.h"

#include "../Engine/WorldManager.h"
#include "../NetworkLibrary/MyProtocol.h"

void UIGotoMainClickFunctor::Execute(UIObject& onwer, float deltaTime)
{
	PacketC2S_RestartGame rg;
	rg.restart = '0';

	WorldManager::GetInstance()->PushSendQueue(
		WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_RestartGame), C2S_RESTART_GAME, &rg.restart), sizeof(PacketC2S_RestartGame));
}
