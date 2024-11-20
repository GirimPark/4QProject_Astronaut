#pragma once
#include "../Engine/World.h"
#include "../Engine/EventListener.h"
#include "../Engine/UIPanel.h"

class UIObject;

class CreditScene 
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
		const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, 
		const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;

private:
	std::shared_ptr<GameObject> m_Camera;
	std::shared_ptr<UIObject> m_CreditUI;
};

