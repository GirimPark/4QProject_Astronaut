#include "pch.h"
#include "UITextInputBox.h"
#include "UIManager.h"

UITextInputBox::UITextInputBox(const std::wstring& name)
	: UIObject(name)
{
	InputManager::GetInstance()->AddInputProcesser(this);
}

UITextInputBox::~UITextInputBox()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);
}

void UITextInputBox::Initialize()
{
	__super::Initialize();
}

void UITextInputBox::Update(float deltaTime)
{
	__super::Update(deltaTime);

	//UpdateCursorBlinkTimer(deltaTime);

	m_uiText->SetText(m_inputText + L'|');

	m_uiText->Update(deltaTime);
}

void UITextInputBox::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{

	for (int i = static_cast<int>(Keyboard::A); i <= static_cast<int>(Keyboard::Z); ++i)
	{
		Keyboard::Keys key = static_cast<Keyboard::Keys>(i);

		if (KeyTracker.IsKeyPressed(key))
		{
			char character = 'A' + (i - static_cast<int>(Keyboard::Keys::A));
			AppendCharacter(character);
			return;
		}

	}

	if (KeyTracker.IsKeyPressed(Keyboard::Keys::Back))
	{
		RemoveLastCharacter();
		return;
	}

	if (KeyTracker.IsKeyPressed(Keyboard::Keys::Enter))
	{
		// 엔터 입력 시 동작 구현
	}
}

void UITextInputBox::AppendCharacter(char character)
{
	if (m_inputText.length() < 4)
	{
		m_inputText += character;
	}
}

void UITextInputBox::RemoveLastCharacter()
{
	if (!m_inputText.empty())
	{
		m_inputText.pop_back();
	}
}

//void UITextInputBox::UpdateCursorBlinkTimer(float deltaTime)
//{
//	m_cursorBlinkTimer += deltaTime;
//	if (m_cursorBlinkTimer >= m_cursorBlinkInterval)
//	{
//		if (m_showCursor)
//		{
//			m_cursor = '|';
//		}
//		else
//		{
//			m_cursor = ' ';
//		}
//		m_showCursor = !m_showCursor;
//		m_cursorBlinkTimer -= m_cursorBlinkInterval;
//	}
//}
//
//void UITextInputBox::UpdateDelay(float deltaTime)
//{
//	if (!m_firstInput && m_delayTimer < INPUT_DELAY)
//	{
//		m_delayTimer += deltaTime;
//	}
//	else
//	{
//		m_delayComplete = true;
//	}
//}