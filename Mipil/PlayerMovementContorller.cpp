#include "pch.h"
#include "PlayerMovementContorller.h"
#include "PlayerFreeObject.h"
#include "PlayerFreeCamera.h"

#include "../Engine/RigidDynamicComponent.h"

void PlayerMovementContorller::HandleInput(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	PxVec3 moveDirection(0.f, 0.f, 0.f);

	//m_pitch = 0.f;
	//m_roll = 0.f;

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::W))
	{
		m_isPushedButton = true;
		moveDirection -= PxVec3(m_playerForward.x, 0.f, m_playerForward.z);

		m_pitch = std::lerp(m_pitch, -0.3f, m_smoothingFactor);

	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::S))
	{
		m_isPushedButton = true;
		moveDirection += PxVec3(m_playerForward.x, 0.f, m_playerForward.z);

		m_pitch = std::lerp(m_pitch, 0.3f, m_smoothingFactor);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::A))
	{
		m_isPushedButton = true;
		moveDirection -= PxVec3(m_playerRight.x, 0.f, m_playerRight.z);

		m_roll = std::lerp(m_roll, -0.3f, m_smoothingFactor);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::D))
	{
		m_isPushedButton = true;
		moveDirection += PxVec3(m_playerRight.x, 0.f, m_playerRight.z);

		m_roll = std::lerp(m_roll, 0.3f, m_smoothingFactor);
	}

	if (moveDirection.magnitudeSquared() > 0.f)
	{
		m_rigidDynamic->clearTorque();
		m_rigidDynamic->setAngularVelocity(physx::PxVec3(0.f, 0.f, 0.f));

		moveDirection.normalize();
		m_rigidDynamic->addForce(moveDirection * m_moveSpeed, PxForceMode::eVELOCITY_CHANGE);
	}

	m_pitch = std::lerp(m_pitch, 0.0f, m_smoothingFactor);
	m_roll = std::lerp(m_roll, 0.0f, m_smoothingFactor);

	PxVec3 linearVelocity = m_rigidDynamic->getLinearVelocity();
	PxVec3 dampingForce = -linearVelocity * m_dampingFactor;

	m_rigidDynamic->addForce(dampingForce, PxForceMode::eFORCE);
}

void PlayerMovementContorller::Update(float deltaTime)
{
	if (m_rigidDynamic == nullptr)
	{
		m_rigidDynamic = m_owner.lock()->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic();
	}

	if (m_playerObject == nullptr)
	{
		m_playerObject = dynamic_cast<PlayerFreeObject*>(m_owner.lock().get());
		m_playerObject->Fire(false);
	}

	if (m_isPushedButton)
	{
		if (m_isFire == false)
		{
			m_playerObject->Fire(true);
			m_isFire = true;
		}
		auto& worldMatrix = m_playerObject->GetCamera()->GetTransform();
		m_playerForward = worldMatrix.Forward();
		m_playerRight = worldMatrix.Right();
		m_playerForward.Normalize();

		m_yaw = std::atan2(m_playerForward.x, m_playerForward.z);

		auto camQuat = Math::Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
		camQuat.Normalize();
		auto playerPos = m_rigidDynamic->getGlobalPose();
		m_rigidDynamic->setGlobalPose(physx::PxTransform(playerPos.p, physx::PxQuat(camQuat.x, camQuat.y, camQuat.z, camQuat.w)));

		m_isPushedButton = false;
	}
	else
	{
		if (m_isFire == true)
		{
			m_playerObject->Fire(false);
			m_isFire = false;
		}
		auto camQuat = Math::Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
		camQuat.Normalize();
		auto playerPos = m_rigidDynamic->getGlobalPose();
		m_rigidDynamic->setGlobalPose(physx::PxTransform(playerPos.p, physx::PxQuat(camQuat.x, camQuat.y, camQuat.z, camQuat.w)));
	}
}

