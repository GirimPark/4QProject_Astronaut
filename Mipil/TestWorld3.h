#pragma once
#include "../Engine/World.h"
#include "../Engine/InputManager.h"
#include "../Engine/EventListener.h"

class TestWorld3 :
    public World
	, public EventListener
	, public InputProcesser
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void OnEnter() override;
	void OnExit() override;

	// InputProcesser을(를) 통해 상속됨
	void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;

	// EventListener을(를) 통해 상속됨
	void HandleEvent(Event* event) override;

private:
	std::shared_ptr<GameObject> m_Camera;
	std::shared_ptr<GameObject> m_plane;
	std::shared_ptr<GameObject> m_staticActor;
	std::shared_ptr<GameObject> m_dynamicActor;

};

