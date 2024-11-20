#include "pch.h"
#include "InputManager.h"
#include "CommonApp.h"

InputManager* InputManager::m_pInstance = nullptr;

void InputManager::AddInputProcesser(InputProcesser* inputProcesser)
{
	m_InputProcessers.push_back(inputProcesser);
}

void InputManager::RemoveInputProcesser(InputProcesser* inputProcesser)
{
	m_InputProcessers.remove(inputProcesser);
}

void InputManager::Initialize()
{
	m_Keyboard = std::make_unique<DirectX::Keyboard>();
	m_Mouse = std::make_unique<DirectX::Mouse>();
	m_Mouse->SetWindow(CommonApp::m_pInstance->GetHwnd());
}

void InputManager::Finalize()
{
	delete m_pInstance;
}

void InputManager::Update(float DeltaTime)
{
	m_MouseState = m_Mouse->GetState();
	m_MouseStateTracker.Update(m_MouseState);

	m_KeyboardState = m_Keyboard->GetState();
	m_KeyboardStateTracker.Update(m_KeyboardState);

	for (auto& it : m_InputProcessers)
	{
		it->OnInputProcess(m_KeyboardState, m_KeyboardStateTracker, m_MouseState, m_MouseStateTracker);
	}
}

