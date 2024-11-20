#include "pch.h"
#include "PlayerState.h"

#include "PlayerFSM.h"
#include "PlayerObject.h"
#include "TestPlayerObject.h"

#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

/// PlayerState
void PlayerState::OnEnter()
{
	PlayerObject* obj = m_pOwnerFSM->GetOwnerObject();

	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_ANIMATION, obj, m_name.c_str());
}

void PlayerState::OnExit()
{
	EventManager::GetInstance()->SendEvent(eEventType::PLAY_FIRE_PARTICLE, m_pOwnerFSM->GetOwnerObject(), false);
}

void PlayerState::DecideFSMType()
{
	m_pOwnerFSM = dynamic_cast<PlayerFSM*>(m_pParentFSM);
}


/// State_Idle
State_Idle::State_Idle()
{
	AddTransition(&m_toTurnWait);
	AddTransition(&m_toTurnMove);
	AddTransition(&m_toTurnUp);
}

void State_Idle::OnEnter()
{
	PlayerState::OnEnter();

	PlayerObject* obj = m_pOwnerFSM->GetOwnerObject();

	obj->FallToGround();
}

void State_Idle::SetOwnerType(bool isHostPlayer)
{
	m_toTurnWait.SetOwnerPlayer(isHostPlayer);
	m_toTurnMove.SetOwnerPlayer(isHostPlayer);
	m_toTurnUp.SetOwnerPlayer(isHostPlayer);
}


/// State_Floating
State_Floating::State_Floating()
{
	AddTransition(&m_toTurnWait);
	AddTransition(&m_toTurnFloating);
	AddTransition(&m_toTurnMove);
	AddTransition(&m_toTurnUp);
}

void State_Floating::OnEnter()
{
	PlayerState::OnEnter();

	EventManager::GetInstance()->SendEvent(eEventType::PLAY_FIRE_PARTICLE, m_pOwnerFSM->GetOwnerObject(), true);
}

void State_Floating::SetOwnerType(bool isHostPlayer)
{
	m_toTurnWait.SetOwnerPlayer(isHostPlayer);
	m_toTurnFloating.SetOwnerPlayer(isHostPlayer);
	m_toTurnMove.SetOwnerPlayer(isHostPlayer);
	m_toTurnUp.SetOwnerPlayer(isHostPlayer);
}


/// State_TurnWait
State_TurnWait::State_TurnWait()
{
	AddTransition(&m_toIdle);
}

void State_TurnWait::SetOwnerType(bool isHostPlayer)
{
	m_toIdle.SetOwnerPlayer(isHostPlayer);
}


/// State_TurnMove
State_TurnMove::State_TurnMove()
{
	AddTransition(&m_toIdle);
	AddTransition(&m_toCrash);
	AddTransition(&m_toGetItem);
}

void State_TurnMove::OnEnter()
{
	PlayerState::OnEnter();

	SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Spaceship_Move.mp3");
}

void State_TurnMove::SetOwnerType(bool isHostPlayer)
{
	m_toIdle.SetOwnerPlayer(isHostPlayer);
	m_toCrash.SetOwnerPlayer(isHostPlayer);
	m_toGetItem.SetOwnerPlayer(isHostPlayer);
}


/// State_TurnUp
State_TurnUp::State_TurnUp()
{
	AddTransition(&m_toIdle);
	AddTransition(&m_toFloating);
	AddTransition(&m_toCrash);
	AddTransition(&m_toGetItem);
}

void State_TurnUp::OnEnter()
{
	PlayerState::OnEnter();

	EventManager::GetInstance()->SendEvent(eEventType::PLAY_FIRE_PARTICLE, m_pOwnerFSM->GetOwnerObject(), true);

	SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Spaceship_Up.mp3");
}

void State_TurnUp::SetOwnerType(bool isHostPlayer)
{
	m_toIdle.SetOwnerPlayer(isHostPlayer);
	m_toFloating.SetOwnerPlayer(isHostPlayer);
	m_toCrash.SetOwnerPlayer(isHostPlayer);
	m_toGetItem.SetOwnerPlayer(isHostPlayer);
}


/// State_TurnFloating
State_TurnFloating::State_TurnFloating()
{
	AddTransition(&m_toFloating);
}

void State_TurnFloating::OnEnter()
{
	PlayerState::OnEnter();

	EventManager::GetInstance()->SendEvent(eEventType::PLAY_FIRE_PARTICLE, m_pOwnerFSM->GetOwnerObject(), true);
}

void State_TurnFloating::SetOwnerType(bool isHostPlayer)
{
	m_toFloating.SetOwnerPlayer(isHostPlayer);
}


/// State_Crash
State_Crash::State_Crash()
{
	AddTransition(&m_toTurnMove);
	AddTransition(&m_toTurnUp);
}

void State_Crash::OnEnter()
{
	PlayerObject* obj = m_pOwnerFSM->GetOwnerObject();
	
	if (obj->GetHostPlayer())
	{
		std::string str = "HostPlayer : ";
		str.append(m_name.c_str());
		LOG_CONTENTS(str.c_str());
	}
	else
	{
		std::string str = "GuestPlayer : ";
		str.append(m_name.c_str());
		LOG_CONTENTS(str.c_str());
	}

	// 머터리얼 갈아끼기
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/OP_Rocket_Damaged.mp3");
	obj->CrashMaterial(true, 2.f);
}

void State_Crash::SetOwnerType(bool isHostPlayer)
{
	m_toTurnMove.SetOwnerPlayer(isHostPlayer);
	m_toTurnUp.SetOwnerPlayer(isHostPlayer);
}


/// State_GetItem
State_GetItem::State_GetItem()
{
	AddTransition(&m_toTurnMove);
	AddTransition(&m_toTurnUp);
}

void State_GetItem::OnEnter()
{
	PlayerObject* obj = m_pOwnerFSM->GetOwnerObject();

	if (obj->GetHostPlayer())
	{
		std::string str = "HostPlayer : ";
		str.append(m_name.c_str());
		LOG_CONTENTS(str.c_str());
	}
	else
	{
		std::string str = "GuestPlayer : ";
		str.append(m_name.c_str());
		LOG_CONTENTS(str.c_str());
	}

	// 머터리얼 갈아끼기
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Spaceship_Get.mp3");
	obj->SmaileMaterial(true, 2.f);
}

void State_GetItem::SetOwnerType(bool isHostPlayer)
{
	m_toTurnMove.SetOwnerPlayer(isHostPlayer);
	m_toTurnUp.SetOwnerPlayer(isHostPlayer);
}
