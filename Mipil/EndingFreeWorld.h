#pragma once
#include "../Engine/World.h"
#include "../Engine/EventListener.h"
#include "../Engine/InputManager.h"
#include <queue>

class DivisionMap;
class UIPanel;
class UIText;
class PlayerFreeCamera;
class PlayerFreeObject;
class RigidDynamicComponent;
class SkeletalMeshComponent;

class EndingFreeWorld
	: public World
	, public EventListener
	, public InputProcesser
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void UpdateAnimation();
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
	float CalcDistance(Math::Vector3 startPos, Math::Vector3 destPos);
	void SetDialogue(float trigger[]);
	void GotoEndingCutScene(char* pMsg);


public:
	void TriggerDim(bool dim);
	void StopMove();
	void DeletePlayerController();

private:
	// UI
	std::shared_ptr<UIPanel> m_dimUI;
	std::shared_ptr<UIText> m_DistanceText;

	float m_remainDistance = 0.f;
	float m_processDistance = 0.f;
	float m_totlaDistance = 0.f;

	bool m_isTrigger = false;
	bool m_isMoving = false;

	Math::Vector3 m_destPosition;

	bool m_isNextWorldTimer = false;
	float m_nextWorldTime = 0.0f;

	// fade in out
	std::shared_ptr<UIPanel> m_tFadeInOut;
	float m_tFadeDurationTime;

	std::shared_ptr<PlayerFreeObject> m_player;
	std::shared_ptr<PlayerFreeCamera> m_mainCamera;

	RigidDynamicComponent* m_playerRigid;
	SkeletalMeshComponent* m_playerMesh;

	std::queue<std::pair <std::shared_ptr<UIPanel> ,float >>  m_dialogueQueue;
 };

