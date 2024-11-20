#include "pch.h"
#include "FSMComponent.h"

#include "FSM.h"

void FSMComponent::AttachFSM(std::shared_ptr<FSM> FSM)
{
	m_FSM = FSM;
}

void FSMComponent::Initialize()
{
}

void FSMComponent::Update(float deltaTime)
{
	m_FSM->Update(deltaTime);
}
