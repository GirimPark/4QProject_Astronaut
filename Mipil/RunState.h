#pragma once
#include "../Engine/FSMState.h"

#include "PlayerTransition.h"

class PlayerFSM;
class RunToIdle;

class RunState
	: public FSMState
{
public:
	RunState();

	void OnEnter() final;

	void DecideFSMType() override;

private:
	RunToIdle m_toIdleTransition;

	PlayerFSM* m_pOwnerFSM;
};

