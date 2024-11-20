#include "pch.h"
#include "PlayerTransition.h"

#include "../Engine/EventManager.h"

PlayerTransition::PlayerTransition()
{
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_TURN, this);
	EventManager::GetInstance()->RegisterListener(eEventType::GET_THIS_ACTION, this);
	EventManager::GetInstance()->RegisterListener(eEventType::CHECK_GROUND, this);
	EventManager::GetInstance()->RegisterListener(eEventType::CHECK_CRASH, this);
	EventManager::GetInstance()->RegisterListener(eEventType::CHECK_GET_ITEM, this);
	EventManager::GetInstance()->RegisterListener(eEventType::SET_PRE_STATE, this);
}

PlayerTransition::~PlayerTransition()
{
	EventManager::GetInstance()->UnregisterListener(eEventType::CHANGE_TURN, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::GET_THIS_ACTION, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::CHECK_GROUND, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::CHECK_CRASH, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::CHECK_GET_ITEM, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::SET_PRE_STATE, this);
}

void PlayerTransition::HandleEvent(Event* event)
{
	switch (event->eventID)
	{
	case eEventType::CHANGE_TURN:
		if(m_bHostPlayers == event->GetData<Event_TurnInfo>().isHostPlayer)
			m_bTurn = event->GetData<Event_TurnInfo>().isTurn;
		return;
	case eEventType::GET_THIS_ACTION:
		m_action = event->GetData<Vector3>();
		return;
	case eEventType::CHECK_GROUND:
		if (m_bHostPlayers == event->GetData<Event_GroundInfo>().isHostPlayer)
			m_bGround = event->GetData<Event_GroundInfo>().isGround;
		return;
	case eEventType::CHECK_CRASH:
		m_bOnCrash = event->GetData<bool>();
		return;
	case eEventType::CHECK_GET_ITEM:
		m_bOnGetItem = event->GetData<bool>();
		return;
	case eEventType::SET_PRE_STATE:
		m_preState = event->GetData<const char*>();
		return;
	default:
		assert(true);
		break;
	}
}

void PlayerTransition::SetOwnerPlayer(bool isHostPlayer)
{
	m_bHostPlayers = isHostPlayer;
}

bool PlayerTransition::IsMoveState(Vector3 vec)
{
	if (vec == grid::X_DOWN || vec == grid::X_UP || vec == grid::Z_DOWN || vec == grid::Z_UP)
		return true;
	else
		return false;
}

void PlayerTransition::SetPreStateForAll(std::string preState)
{
	EventManager::GetInstance()->SendImmediateEvent(eEventType::SET_PRE_STATE, nullptr, preState.c_str());
}

void PlayerTransition::InitCrashForAll()
{
	EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_CRASH, nullptr, false);
}

void PlayerTransition::InitGetItemForAll()
{
	EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_GET_ITEM, nullptr, false);
}
