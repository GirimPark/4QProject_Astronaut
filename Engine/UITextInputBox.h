#pragma once
#include "UIObject.h"
#include "UIText.h";
#include "InputManager.h"


class UITextInputBox :
	public UIObject,
	public InputProcesser
{
public:
	UITextInputBox(const std::wstring& name);
	virtual ~UITextInputBox();

public:
	void SetUITextObject(std::shared_ptr<UIText> uiTextObject) { m_uiText = uiTextObject; }

	void SetTextFont(std::wstring_view fontName) { m_uiText->SetFont(fontName); }
	void SetTextFontSize(float size) { m_uiText->SetFontSize(size); }
	void SetTextColor(D2D1::ColorF color) { m_uiText->SetColor(color); }
	void SetTextFontType(DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch) { m_uiText->SetFontType(weight, style, stretch); }

	void SetTextBoxPosition(const Math::Vector2& position) { m_uiText->SetPosition(position); }
	void SetTextBoxSize(const Math::Vector2& size) { m_uiText->SetSize(size); }

public:
	virtual void Initialize();
	virtual void Update(float deltaTime);
	void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;

public:
	void AppendCharacter(char character);
	void RemoveLastCharacter();
	//void UpdateCursorBlinkTimer(float deltaTime);
	//void UpdateDelay(float deltaTime);


private:
	std::wstring m_inputText;
	std::shared_ptr<UIText> m_uiText;

	const float m_cursorBlinkInterval = 0.5f;
	float m_cursorBlinkTimer = 0.0f;
	std::wstring m_cursor;
	bool m_showCursor;

	//bool m_isInputActive;

	//bool m_firstInput;
	//bool m_delayComplete;
	//float m_delayTimer;
	//const float INPUT_DELAY = 0.3f;
};

