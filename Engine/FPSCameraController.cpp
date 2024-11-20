#include "pch.h"
#include "FPSCameraController.h"

#include "GameObject.h"
#include "InputManager.h"

void FPSCameraController::HandleInput(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	auto& ownerLocalMatrix = m_owner.lock()->GetTransform();
	Math::Vector3 forward = ownerLocalMatrix.Forward();
	Math::Vector3 right = ownerLocalMatrix.Right();

	if (KeyTracker.IsKeyPressed(Keyboard::Keys::Home))
	{
		m_owner.lock()->SetPosition(Math::Vector3::Zero);
		m_pitch = 0.f;
		m_yaw = 0.f;
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::W))
	{
		AddInputVector(-forward);
	}

	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::S))
	{
		AddInputVector(forward);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::A))
	{
		AddInputVector(-right);
	}

	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::D))
	{
		AddInputVector(right);
	}

	if (MouseState.xButton1)
	{
		AddInputVector(Math::Vector3::Down);
	}

	else if (MouseState.xButton2)
	{
		AddInputVector(Math::Vector3::Up);
	}

	InputManager::GetInstance()->GetMouse()->SetMode(MouseState.rightButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

	if (MouseState.positionMode == Mouse::MODE_RELATIVE)
	{
		Math::Vector3 delta = Math::Vector3(float(MouseState.x), float(MouseState.y), 0.f) * m_rotationSpeed;
		AddPitch(delta.y);
		AddYaw(delta.x);

		//// 원하는 물체를 향하도록 카메라의 방향을 설정
		//Math::Vector3 TargetVec = { 0,0,0 };
		//DirectX::XMVECTOR forward = DirectX::XMVectorSubtract(TargetVec, m_owner.lock()->GetLocalPosition());

		//// 필요에 따라 카메라의 업 벡터를 설정하여 원하는 방향으로 회전
		//DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // 예시: y축이 위쪽이라고 가정
		//DirectX::XMVECTOR right = DirectX::XMVector3Cross(up, forward);
		//up = DirectX::XMVector3Cross(forward, right);

		//// 카메라 행렬 생성
		//DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookToLH(m_owner.lock()->GetLocalPosition(), forward, up);
	}
}

void FPSCameraController::Update(float deltaTime)
{
	const auto& owner = m_owner.lock();

	if (m_InputVector.Length() > 0.0f)
	{
		owner->SetPosition(owner->GetLocalPosition() + (m_InputVector * m_movementSpeed * deltaTime));
		m_InputVector = Math::Vector3::Zero;
	}

	owner->SetRotation(Math::Vector3(XMConvertToDegrees(m_pitch), XMConvertToDegrees(m_yaw), 0.0f));
}

void FPSCameraController::AddInputVector(const Math::Vector3& input)
{
	m_InputVector += input;
	m_InputVector.Normalize();
}

void FPSCameraController::AddPitch(const float value)
{
	m_pitch += value;
	if (m_pitch > XM_PI)
	{
		m_pitch -= XM_2PI;
	}
	else if (m_pitch < -XM_PI)
	{
		m_pitch += XM_2PI;
	}
}

void FPSCameraController::AddYaw(const float value)
{
	m_yaw += value;
	if (m_yaw > XM_PI)
	{
		m_yaw -= XM_2PI;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_2PI;
	}
}

void FPSCameraController::SetLookTarget(bool isLookTarget)
{
	m_bIsLookTarget = isLookTarget;
}
