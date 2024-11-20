#pragma once
#include "../Engine/World.h"
#include "../Engine/EventListener.h"
#include "../Engine/InputManager.h"
#include "../Engine/UIPanel.h"
#include "../Engine/UIText.h"

#include <queue>
#include <stack>
#include <memory>

class StartingCutScene
	: public World
	, public EventListener
	, public InputProcesser
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void OnEnter() override;
	void OnExit() override;

public:
	void HandleEvent(Event* event) override;

	void OnInputProcess(
		const Keyboard::State& KeyState,
		const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState,
		const Mouse::ButtonStateTracker& MouseTracker) override;
	
public:
	// text
	std::queue<std::wstring> m_dialogueQueue;
	std::shared_ptr<UIText> m_Text;
	std::wstring m_nowSentence;
	std::wstring m_nowWord;
	size_t m_nowTextIndex = 0;

	// image
	std::stack<std::shared_ptr<UIPanel>> m_imageStack;

	// fade in out
	std::shared_ptr<UIPanel> m_tFadeInOut;
	float m_tFadeDurationTime;

	bool m_bIsCanStartFade = true;
	bool m_bIsCanFadeOut = true;
	bool m_bIsCanFadeIn = true;
	bool m_bStartCount = false;
	//
	float m_skipDelayTime;
	float m_delayTime;
};

