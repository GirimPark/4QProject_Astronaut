#include "pch.h"
#include "PlayerFreeCamera.h"
#include "PlayerFreeCameraContorller.h"
#include "../Engine/CameraComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/FPSCameraController.h"

void PlayerFreeCamera::Initialize()
{
	auto camera = CreateComponent<CameraComponent>("Camera_Component").lock();
	SetRootComponent(camera);

	auto controller = CreateComponent<ControllerComponent>("Cam_Controller_Component").lock();
	//controller->CreateController<FPSCameraController>();
	controller->CreateController<PlayerFreeCameraContorller>();

	GameObject::Initialize();
}

void PlayerFreeCamera::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}