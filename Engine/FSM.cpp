#include "pch.h"
#include "FSM.h"

#include "FSMState.h"

void FSM::AddState(std::string_view name, FSMState* state)
{
	m_stateMap.insert(std::make_pair(name, state));
	state->SetName(name);
	state->SetParentFSM(this);
}

void FSM::AddAnimation(std::string_view stateName, std::string_view animationName)
{
	m_stateAnimationMap.emplace(stateName, animationName);
}

void FSM::SetDefaultState(std::string_view name)
{
	m_pCurState = m_stateMap.find(std::string(name))->second;
}

std::string& FSM::GetAnimationName(std::string_view stateName)
{
	return m_stateAnimationMap.find(std::string(stateName))->second;
}

std::string& FSM::GetMaterialName(std::string_view stateName)
{
	return m_stateMaterialMap.find(std::string(stateName))->second;
}

void FSM::Update(float deltaTime)
{
	if (m_pCurState->CheckTransition())
	{
		// 만족하는 Transition이 있으면 상태 전환
		m_pCurState->OnExit();
		m_pCurState = m_pNextState;
		m_pCurState->OnEnter();
	}
}
