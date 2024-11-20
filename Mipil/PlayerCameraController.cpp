#include "pch.h"
#include <algorithm>
#include "../Engine/InputManager.h"
#include "../Engine/SceneComponent.h"
#include "PlayerCameraController.h"
#include "PlayerCamera.h"


PlayerCameraController::PlayerCameraController()
{
}

PlayerCameraController::~PlayerCameraController()
{
}

void PlayerCameraController::HandleInput(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	if (MouseState.positionMode == Mouse::MODE_RELATIVE)
	{
		if (m_enterRelative)
		{
			m_currentYaw = 0.f;
			m_currentPitch = 0.f;

			m_enterRelative = false;
		}

		if (MouseState.rightButton == Mouse::ButtonStateTracker::HELD)
		{
			m_currentYaw += static_cast<float>(MouseState.x) * m_playerCamera->m_rotationSpeed;
			m_currentPitch += static_cast<float>(MouseState.y) * m_playerCamera->m_rotationSpeed;

			m_currentPitch = std::clamp(m_currentPitch, m_playerCamera->m_minPitch, m_playerCamera->m_maxPitch);

			m_offset = Math::Vector3{
				m_playerCamera->m_distance * std::sin(XMConvertToRadians(180.f + m_currentYaw)) * std::cos(XMConvertToRadians(20.f + m_currentPitch)),
				m_playerCamera->m_distance * std::sin(XMConvertToRadians(20.f + m_currentPitch)),
				m_playerCamera->m_distance * std::cos(XMConvertToRadians(180.f + m_currentYaw)) * std::cos(XMConvertToRadians(20.f + m_currentPitch))
			};
		}
	}

	if (MouseState.positionMode == Mouse::MODE_ABSOLUTE)
	{
		m_offset = m_baseOffset;
		m_enterRelative = true;
	}
}

void PlayerCameraController::Update(float deltaTime)
{
	if (m_playerCamera == nullptr)
	{
		m_playerCamera = dynamic_cast<PlayerCamera*>(m_owner.lock().get());

		Math::Vector3 playerRotationEuler = m_playerCamera->m_subject->GetLocalRotation();

		m_currentYaw = playerRotationEuler.y;
		m_currentPitch = playerRotationEuler.x;

		m_baseOffset = {
		m_playerCamera->m_distance * std::sin(XMConvertToRadians(180.f)) * std::cos(XMConvertToRadians(20.f)),
		m_playerCamera->m_distance * std::sin(XMConvertToRadians(20.f)),
		m_playerCamera->m_distance * std::cos(XMConvertToRadians(180.f)) * std::cos(XMConvertToRadians(20.f))
		};
	}

	m_newPosition = m_playerCamera->m_subject->GetLocalPosition() + m_offset;
	SetLookAt();
}

void PlayerCameraController::SetLookAt()
{
	Math::Vector3 directionVector = (m_playerCamera->m_subject->GetLocalPosition() + m_playerCamera->m_lookAtOffset) - m_newPosition;
	directionVector.Normalize();

	float yaw = std::atan2(directionVector.x, directionVector.z);
	float pitch = std::asin(directionVector.y);

	m_playerCamera->SetPosition(m_newPosition);
	m_playerCamera->SetRotation({ -XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), 0.f });

}