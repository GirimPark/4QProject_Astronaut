#pragma once
#include "Controller.h"

class FPSCameraController :
	public Controller
{
public:
	virtual void HandleInput(
		const Keyboard::State& KeyState,
		const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState,
		const Mouse::ButtonStateTracker& MouseTracker) final;
	virtual void Update(float deltaTime) final;

private:
	void AddInputVector(const Math::Vector3& input);
	void AddPitch(const float value);
	void AddYaw(const float value);

	// 재현
	void SetLookTarget(bool isLookTarget);

private:
	float m_rotationSpeed = 0.004f;
	float m_movementSpeed = 1000.f;

	Math::Vector3 m_InputVector;
	float m_pitch = 0.0f; // radian
	float m_yaw = 0.0f;

	// 재현
	bool m_bIsLookTarget = false;
};

