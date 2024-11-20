#include "pch.h"
#include "PlayerCamera.h"
#include "../Engine/CameraComponent.h"
#include "../Engine/ControllerComponent.h"
#include "PlayerCameraController.h"
#include "../Engine/FPSCameraController.h"

void PlayerCamera::Initialize()
{
	auto camera = CreateComponent<CameraComponent>("Camera_Component").lock();
	SetRootComponent(camera);

	auto controller = CreateComponent<ControllerComponent>("Cam_Controller_Component").lock();
	//controller->CreateController<FPSCameraController>();
	controller->CreateController<PlayerCameraController>();

	GameObject::Initialize();
}

void PlayerCamera::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}