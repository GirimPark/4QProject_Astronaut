#include "pch.h"
#include "RunState.h"

#include "PlayerFSM.h"
#include "PlayerTransition.h"
#include "PlayerObject.h"

#include "../Engine/EventManager.h"

RunState::RunState()
{
	AddTransition(&m_toIdleTransition);

	EventManager::GetInstance()->RegisterListener(eEventType::TRANSPER_MOVEMENT, &m_toIdleTransition);
}

void RunState::OnEnter()
{
	// 애니메이션 변경 이벤트
	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_ANIMATION, m_pOwnerFSM->GetOwnerObject(), m_pParentFSM->GetAnimationName(m_name).c_str());
}

void RunState::DecideFSMType()
{
	m_pOwnerFSM = dynamic_cast<PlayerFSM*>(m_pParentFSM);
}
