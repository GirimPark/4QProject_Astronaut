#include "pch.h"
#include <algorithm>
#include "PlayerFreeCameraContorller.h"
#include "../Engine/InputManager.h"
#include "../Engine/SceneComponent.h"
#include "PlayerFreeCamera.h"


PlayerFreeCameraContorller::PlayerFreeCameraContorller()
{
}

PlayerFreeCameraContorller::~PlayerFreeCameraContorller()
{
}

void PlayerFreeCameraContorller::HandleInput(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	if (MouseState.positionMode == Mouse::MODE_RELATIVE) {
		m_currentYaw += static_cast<float>(MouseState.x) * m_playerCamera->m_rotationSpeed;
		m_currentPitch += static_cast<float>(MouseState.y) * m_playerCamera->m_rotationSpeed;

		m_currentPitch = std::clamp(m_currentPitch, m_playerCamera->m_minPitch, m_playerCamera->m_maxPitch);

		m_offset = {
			m_playerCamera->m_distance * std::sin(XMConvertToRadians(m_currentYaw)) * std::cos(XMConvertToRadians(m_currentPitch)),
			m_playerCamera->m_distance * std::sin(XMConvertToRadians(m_currentPitch)),
			m_playerCamera->m_distance * std::cos(XMConvertToRadians(m_currentYaw)) * std::cos(XMConvertToRadians(m_currentPitch))
		};
	}
}

void PlayerFreeCameraContorller::Update(float deltaTime)
{
	if (m_playerCamera == nullptr)
	{
		m_playerCamera = dynamic_cast<PlayerFreeCamera*>(m_owner.lock().get());

		Math::Vector3 playerRotationEuler = m_playerCamera->m_subject->GetLocalRotation();

		m_currentYaw = playerRotationEuler.y;
		m_currentPitch = playerRotationEuler.x;
	}

	m_newPosition = m_playerCamera->m_subject->GetLocalPosition() + m_offset;
	SetLookAt();
}

void PlayerFreeCameraContorller::SetLookAt()
{
	Math::Vector3 directionVector = (m_playerCamera->m_subject->GetLocalPosition() + m_playerCamera->m_lookAtOffset) - m_newPosition;
	directionVector.Normalize();

	float yaw = std::atan2(directionVector.x, directionVector.z);
	float pitch = std::asin(directionVector.y);

	m_playerCamera->SetPosition(m_newPosition);
	m_playerCamera->SetRotation({ -XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), 0.f });

}