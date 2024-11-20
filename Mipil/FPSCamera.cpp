#include "pch.h"
#include "FPSCamera.h"
#include "../Engine/CameraComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/FPSCameraController.h"

void FPSCamera::Initialize()
{
	auto camera = CreateComponent<CameraComponent>("Camera_Component").lock();
	camera->SetLocalPosition({ 50.f, 100.f, -500.f });
	SetRootComponent(camera);

	auto controller = CreateComponent<ControllerComponent>("Cam_Controller_Component").lock();
	controller->CreateController<FPSCameraController>();

	GameObject::Initialize();
}

void FPSCamera::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
