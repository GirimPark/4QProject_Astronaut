#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition.h"

class PlayerFSM;

class IdleState
	: public FSMState
{
public:
	IdleState();

	void OnEnter() final;

	void DecideFSMType() override;
	PlayerTransition* GetParentTransition() const { return m_pParentTransition; }

private:
	IdleToRun m_toRunTransition;

	PlayerFSM* m_pOwnerFSM = nullptr;
	PlayerTransition* m_pParentTransition;
};

