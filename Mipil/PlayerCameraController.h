#pragma once
#include "../Engine/Controller.h"

class PlayerCamera;

class PlayerCameraController :
	public Controller
{
public:
	PlayerCameraController();
	~PlayerCameraController();

	// Controller을(를) 통해 상속됨
	void HandleInput(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;
	void Update(float deltaTime) override;

private:
	void SetLookAt();

private:
	PlayerCamera* m_playerCamera;

	float m_currentYaw = 0.0f;
	float m_currentPitch = 0.0f;

	Math::Vector3 m_newPosition;
	Math::Vector3 m_offset;
	Math::Vector3 m_baseOffset;

	bool m_enterRelative;
};

