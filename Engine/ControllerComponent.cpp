#include "pch.h"
#include "ControllerComponent.h"
#include "InputManager.h"
#include "WorldManager.h"

ControllerComponent::~ControllerComponent()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);
}

void ControllerComponent::Initialize()
{
	InputManager::GetInstance()->AddInputProcesser(this);
}

void ControllerComponent::Update(float deltaTime)
{
	for (auto& it : m_InputOperations)
	{
		it->Update(deltaTime);
	}
}

void ControllerComponent::ClearControllers()
{
	m_InputOperations.clear();
}

void ControllerComponent::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	for (auto& it : m_InputOperations)
	{
		it->HandleInput(KeyState, KeyTracker, MouseState, MouseTracker);
	}
}

