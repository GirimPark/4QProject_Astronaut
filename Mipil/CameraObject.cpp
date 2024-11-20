#include "pch.h"
#include "CameraObject.h"
#include "../Engine/FPSCameraController.h"

void CameraObject::Initialize()
{
	// Camera //
	m_camera = CreateComponent<CameraComponent>("Camera_Component");
	m_camera.lock()->SetLocalPosition({ 50.f, 100.f, -500.f });
	//m_camera.lock()->SetLocalRotation({ 0.f, 180.f, 0.f });
	SetRootComponent(m_camera.lock());

	m_controller = CreateComponent<ControllerComponent>("Cam_Controller_Component");
	m_controller.lock()->CreateController<FPSCameraController>();

	GameObject::Initialize();

	//SetRotation({0.f, 180.f, 0.f});
	SetPosition(m_deferredPosition);
}

void CameraObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
