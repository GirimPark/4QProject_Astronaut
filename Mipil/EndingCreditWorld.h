#pragma once
#include "../Engine/World.h"
#include "../Engine/EventListener.h"

class GameObject;

class EndingCreditWorld 
	: public World
	, public EventListener
{
public:
	EndingCreditWorld();
	~EndingCreditWorld() override;

	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void OnEnter() override;
	void OnExit() override;

public:
	void HandleEvent(Event* event) override;

private:
	std::shared_ptr<GameObject> m_Camera;
};

