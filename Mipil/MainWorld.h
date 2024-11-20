#pragma once

#include "../Engine/World.h"
#include "../Engine/EventListener.h"
#include "../Engine/InputManager.h"

/// <summary>
/// 메인 화면임다
/// </summary>

class MainWorld 
    : public World
    , public EventListener
	, public InputProcesser
{
public:
	MainWorld();
	~MainWorld() override;

	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void OnEnter() override;
	void OnExit() override;

public:
	void HandleEvent(Event* event) override;

	void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;

};

