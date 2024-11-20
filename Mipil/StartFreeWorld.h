#pragma once
#include "../Engine/World.h"
#include "../Engine/EventListener.h"
#include "../Engine/InputManager.h"
#include "../Engine/GameObject.h"
#include "../Engine/UIText.h"
#include "../Engine/UIPanel.h"
#include <queue>

class PlayerFreeCamera;
class PlayerFreeObject;
class RigidDynamicComponent;
class SkeletalMeshComponent;

class StartFreeWorld
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
		const Mouse::ButtonStateTracker& MouseTracker
	) override;

public:
	void TriggerDim(bool dim);
	void StopMove();
	void DeletePlayerController();

	float CalcDistance(Math::Vector3 startPos, Math::Vector3 destPos);
	void SetBearDialogue(float trigger[]);
	void UpdateAnimation();
	void SetRabbitDialogue(float trigger[]);
	void GotoStage1World(char* pMsg);

	// UI
	std::shared_ptr<UIPanel> m_dimUI;
	std::shared_ptr<UIText> m_DistanceText;

	float m_remainDistance = 0.f;
	float m_processDistance = 0.f;
	float m_totlaDistance = 0.f;

	bool m_isTrigger = false;
	bool m_isMoving = false;

	Math::Vector3 m_destPosition;

	// fade in out
	std::shared_ptr<UIPanel> m_tFadeInOut;
	float m_tFadeDurationTime;

	std::shared_ptr<PlayerFreeCamera> m_mainCamera;
	std::shared_ptr<PlayerFreeObject> m_player;

	RigidDynamicComponent* m_playerRigid;
	SkeletalMeshComponent* m_playerMesh;

	std::queue<std::pair <std::shared_ptr<UIPanel>, float >>  m_dialogueQueue;
};

