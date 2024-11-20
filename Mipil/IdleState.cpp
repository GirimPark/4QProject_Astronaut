#include "pch.h"
#include "IdleState.h"

#include "PlayerFSM.h"
#include "PlayerTransition.h"
#include "PlayerObject.h"

#include "../Engine/EventManager.h"
#include "../Engine/FSM.h"

IdleState::IdleState()
{
	AddTransition(&m_toRunTransition);
	m_pParentTransition = &m_toRunTransition;

	EventManager::GetInstance()->RegisterListener(eEventType::TRANSPER_MOVEMENT, &m_toRunTransition);
}

void IdleState::OnEnter()
{
	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_ANIMATION, m_pOwnerFSM->GetOwnerObject(), m_pParentFSM->GetAnimationName(m_name).c_str());
}

void IdleState::DecideFSMType()
{
	m_pOwnerFSM = dynamic_cast<PlayerFSM*>(m_pParentFSM);
}