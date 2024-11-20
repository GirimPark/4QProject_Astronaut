#pragma once
#include "../Engine/FSMState.h"
#include "PlayerTransition.h"

class PlayerFSM;


/// PlayerState
class PlayerState
	: public FSMState
{
public:
	void OnEnter() override;
	void OnExit() override;

	void DecideFSMType() final;
	virtual void SetOwnerType(bool isHostPlayer) abstract;

protected:
	PlayerFSM* m_pOwnerFSM = nullptr;
};


/// State_Idle
class State_Idle
	:public PlayerState
{
public:
	State_Idle();

	void OnEnter() final;
	void SetOwnerType(bool isHostPlayer) final;

private:
	IdleToTurnWait m_toTurnWait;
	IdleToTurnMove m_toTurnMove;
	IdleToTurnUp m_toTurnUp;
};


/// State_Floating
class State_Floating
	:public PlayerState
{
public:
	State_Floating();

	void OnEnter() final;
	void SetOwnerType(bool isHostPlayer) final;

private:
	FloatingToTurnWait m_toTurnWait;
	FloatingToTurnFloating m_toTurnFloating;
	FloatingToTurnMove m_toTurnMove;
	FloatingToTurnUp m_toTurnUp;
};


/// State_TurnWait
class State_TurnWait
	:public PlayerState
{
public:
	State_TurnWait();

	void SetOwnerType(bool isHostPlayer) final;

private:
	TurnWaitToIdle m_toIdle;
};


/// State_TurnMove
class State_TurnMove
	:public PlayerState
{
public:
	State_TurnMove();

	void OnEnter() final;
	void SetOwnerType(bool isHostPlayer) final;

private:
	TurnMoveToIdle m_toIdle;
	TurnMoveToCrash m_toCrash;
	TurnMoveToGetItem m_toGetItem;
};


/// State_TurnUp
class State_TurnUp
	:public PlayerState
{
public:
	State_TurnUp();

	void OnEnter() final;
	void SetOwnerType(bool isHostPlayer) final;

private:
	TurnUpToIdle m_toIdle;
	TurnUpToFloating m_toFloating;
	TurnUpToCrash m_toCrash;
	TurnUpToGetItem m_toGetItem;
};


/// State_TurnFloating
class State_TurnFloating
	:public PlayerState
{
public:
	State_TurnFloating();

	void OnEnter() final;
	void SetOwnerType(bool isHostPlayer) final;

private:
	TurnFloatingToFloating m_toFloating;
};


/// State_Crash
class State_Crash
	:public PlayerState
{
public:
	State_Crash();

	void OnEnter() final;
	void SetOwnerType(bool isHostPlayer) final;

private:
	CrashToTurnMove m_toTurnMove;
	CrashToTurnUp m_toTurnUp;
};


/// State_GetItem
class State_GetItem
	:public PlayerState
{
public:
	State_GetItem();

	void OnEnter() final;
	void SetOwnerType(bool isHostPlayer) final;

private:
	GetItemToTurnMove m_toTurnMove;
	GetItemToTurnUp m_toTurnUp;
};