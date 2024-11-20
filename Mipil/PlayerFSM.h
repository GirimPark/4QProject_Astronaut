#pragma once

#include "../Engine/FSM.h"

#include "PlayerState.h"

class PlayerObject;
class TestPlayerObject;

class PlayerFSM
	: public FSM
{
public:
	PlayerFSM();


	PlayerObject* GetOwnerObject() const { return m_pOwner; }
	TestPlayerObject* GetTestOwnerObject() const { return m_pOwnerTest; }

	void SetOwnerObject(PlayerObject* player);
	void SetOwnerObject(TestPlayerObject* player);

	bool IsIdleState();


private:
	State_Idle m_idle;
	State_Floating m_floating;
	State_TurnWait m_turnWait;
	State_TurnMove m_turnMove;
	State_TurnUp m_turnUp;
	State_TurnFloating m_turnFloating;
	State_Crash m_crash;
	State_GetItem m_getItem;

	PlayerObject* m_pOwner;
	TestPlayerObject* m_pOwnerTest;
};

