#pragma once
#include "../Engine/World.h"
#include "../Engine/EventListener.h"

class GameOverWorld 
	: public World
	, public EventListener
{
public:
	GameOverWorld();
	~GameOverWorld() override;

	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void OnEnter() override;
	void OnExit() override;

	void SetLoading(bool loading) { m_bLoading = loading; }
	// Network
public:
	void Network_Restart(char* pMsg);

public:
	void HandleEvent(Event* event) override;

private:
	bool m_bLoading;
};

