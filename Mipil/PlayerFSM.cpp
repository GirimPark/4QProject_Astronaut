#include "pch.h"
#include "PlayerFSM.h"

#include "PlayerObject.h"
#include "TestPlayerObject.h"

#include "../Engine/SoundManager.h"

PlayerFSM::PlayerFSM()
{
	AddState("State_Idle", &m_idle);
	AddState("State_Floating", &m_floating);
	AddState("State_TurnWait", &m_turnWait);
	AddState("State_TurnMove", &m_turnMove);
	AddState("State_TurnUp", &m_turnUp);
	AddState("State_TurnFloating", &m_turnFloating);
	AddState("State_Crash", &m_crash);
	AddState("State_GetItem", &m_getItem);	

	m_idle.DecideFSMType();
	m_floating.DecideFSMType();
	m_turnWait.DecideFSMType();
	m_turnMove.DecideFSMType();
	m_turnUp.DecideFSMType();
	m_turnFloating.DecideFSMType();
	m_crash.DecideFSMType();
	m_getItem.DecideFSMType();

	SetDefaultState("State_Idle");
}

void PlayerFSM::SetOwnerObject(PlayerObject* player)
{
	m_pOwner = player;

	bool isHostPlayer = player->GetHostPlayer();
	m_idle.SetOwnerType(isHostPlayer);
	m_floating.SetOwnerType(isHostPlayer);
	m_turnWait.SetOwnerType(isHostPlayer);
	m_turnMove.SetOwnerType(isHostPlayer);
	m_turnUp.SetOwnerType(isHostPlayer);
	m_turnFloating.SetOwnerType(isHostPlayer);
	m_crash.SetOwnerType(isHostPlayer);
	m_getItem.SetOwnerType(isHostPlayer);

	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Spaceship_Move.mp3");
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/OP_Rocket_Damaged.mp3");
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Spaceship_Up.mp3");
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Spaceship_Get.mp3");

	if (m_pOwner->GetHostPlayer())
	{
		AddAnimation("State_Idle", "../Resources/FBX/bear_Idle.fbx");
		AddAnimation("State_Floating", "../Resources/FBX/bear_Idle.fbx");
		AddAnimation("State_TurnWait", "../Resources/FBX/bear_Idle_wait.fbx");
		AddAnimation("State_TurnMove", "../Resources/FBX/bear_move.fbx");
		AddAnimation("State_TurnUp", "../Resources/FBX/bear_move.fbx");
		AddAnimation("State_TurnFloating", "../Resources/FBX/bear_Idle.fbx");

		AddAnimation("State_Idle_X", "../Resources/FBX/bear_Idle_spaceship_glass_X.fbx");
		AddAnimation("State_Floating_X", "../Resources/FBX/bear_Idle_spaceship_glass_X.fbx");
		AddAnimation("State_TurnWait_X", "../Resources/FBX/bear_Idle_wait_spaceship_glass_X.fbx");
		AddAnimation("State_TurnMove_X", "../Resources/FBX/bear_move_spaceship_glass_X.fbx");
		AddAnimation("State_TurnUp_X", "../Resources/FBX/bear_move_spaceship_glass_X.fbx");
		AddAnimation("State_TurnFloating_X", "../Resources/FBX/bear_Idle_spaceship_glass_X.fbx");
	}
	else if (!m_pOwner->GetHostPlayer())
	{
		AddAnimation("State_Idle", "../Resources/FBX/rabbit_idle_.fbx");
		AddAnimation("State_Floating", "../Resources/FBX/rabbit_idle_.fbx");
		AddAnimation("State_TurnWait", "../Resources/FBX/rabbit_idle_wait.fbx");
		AddAnimation("State_TurnMove", "../Resources/FBX/rabbit_move_.fbx");
		AddAnimation("State_TurnUp", "../Resources/FBX/rabbit_move_.fbx");
		AddAnimation("State_TurnFloating", "../Resources/FBX/rabbit_idle_.fbx");

		AddAnimation("State_Idle_X", "../Resources/FBX/rabbit_idle_spaceship_glass_X.fbx");
		AddAnimation("State_Floating_X", "../Resources/FBX/rabbit_idle_spaceship_glass_X.fbx");
		AddAnimation("State_TurnWait_X", "../Resources/FBX/rabbit_idle_wait_spaceship_glass_X.fbx");
		AddAnimation("State_TurnMove_X", "../Resources/FBX/rabbit_move_spaceship_glass_X.fbx");
		AddAnimation("State_TurnUp_X", "../Resources/FBX/rabbit_move_spaceship_glass_X.fbx");
		AddAnimation("State_TurnFloating_X", "../Resources/FBX/rabbit_idle_spaceship_glass_X.fbx");
	}
}

void PlayerFSM::SetOwnerObject(TestPlayerObject* player)
{
	m_pOwnerTest = player;

	bool isHostPlayer = m_pOwnerTest->GetHostPlayer();
	m_idle.SetOwnerType(isHostPlayer);
	m_floating.SetOwnerType(isHostPlayer);
	m_turnWait.SetOwnerType(isHostPlayer);
	m_turnMove.SetOwnerType(isHostPlayer);
	m_turnUp.SetOwnerType(isHostPlayer);
	m_turnFloating.SetOwnerType(isHostPlayer);
	m_crash.SetOwnerType(isHostPlayer);
	m_getItem.SetOwnerType(isHostPlayer);
}

bool PlayerFSM::IsIdleState()
{
	if (m_pCurState == &m_idle)
		return true;

	return false;
}
