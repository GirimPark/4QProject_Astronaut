#include "pch.h"
#include "UIManager.h"

UIManager::UIManager()
{
	InputManager::GetInstance()->AddInputProcesser(this);
}
UIManager::~UIManager()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);
}

void UIManager::Initailize()
{
	for (auto& UI : m_UIObjects)
	{
		UI.second->Initialize();
	}
}

void UIManager::Update(float deltaTime)
{
	for (auto& UI : m_UIObjects)
	{
		if (UI.second->GetParent().expired())
			UI.second->Update(deltaTime);
	}

	UpdateHoveredUI(deltaTime);
	UpdateClickedUI(deltaTime);
	UpdateHoverOutUI(deltaTime);
}

void UIManager::ClearFocusedUI()
{
	m_HoveredUIObjects.clear();
	m_ClickedUIObjects.clear();
}

void UIManager::UpdateHoveredUI(float deltaTime)
{
	for (auto& UI : m_HoveredUIObjects)
	{
		UI.lock()->ExecuteHoverFunctors(deltaTime);
	}
	m_HoveredUIObjects.clear();
}

void UIManager::UpdateHoverOutUI(float deltaTime)
{
	for (auto& UI : m_HoverOutUIObjects)
	{
		UI.lock()->ExecuteHoverOutFunctors(deltaTime);
	}
	m_HoverOutUIObjects.clear();
}

void UIManager::UpdateClickedUI(float deltaTime)
{
	for (auto& UI : m_ClickedUIObjects)
	{
		UI.lock()->ExecuteClickFunctors(deltaTime);
	}
	m_ClickedUIObjects.clear();
}

void UIManager::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	int mouseX = MouseState.x;
	int mouseY = MouseState.y;
	Math::Vector2 originPos = { ScreenWidth * 0.5f, ScreenHeight * 0.5f };

	for (auto& UI : m_UIObjects)
	{
		Math::Vector2 pos = UI.second->GetPosition();
		Math::Vector2 dpos = UI.second->GetSize();
		Math::Vector2 dscale = UI.second->GetScale();

		float uiScreenX = originPos.x + (pos.x - dpos.x * (dscale.x * .5f));
		float uiScreenY = originPos.y - (pos.y + dpos.y * (dscale.y * .5f));
		float uiScreenWidth = dpos.x * dscale.x;
		float uiScreenHeight = dpos.y * dscale.y;

		if (mouseX >= uiScreenX && mouseX <= uiScreenX + uiScreenWidth &&
			mouseY >= uiScreenY && mouseY <= uiScreenY + uiScreenHeight)
		{
			m_HoveredUIObjects.push_back(UI.second);
			UI.second->SetHovered(true);

			if (MouseTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
			{
				m_ClickedUIObjects.push_back(UI.second);
			}
		}
		else if (UI.second->IsHovered())
		{
			m_HoverOutUIObjects.push_back(UI.second);
		}
		//Test
		//if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::H))
		//{
		//	m_HoveredUIObjects.push_back(UI);
		//}

		//if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::C))
		//{
		//	m_ClickedUIObjects.push_back(UI);
		//}
		 
		if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::Q))
		{
			auto ui = m_UIObjects.find(L"TestUI");
			if (ui == m_UIObjects.end())
				return;
			ui->second->GetUIInstance().lock()->NotLoopAnimationStart();
		}
	}
}
