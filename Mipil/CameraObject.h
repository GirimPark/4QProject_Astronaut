#pragma once

#include "../Engine/GameObject.h"
#include "../Engine/CameraComponent.h"
#include "../Engine/ControllerComponent.h"

class CameraObject : public GameObject
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::weak_ptr<CameraComponent> m_camera;
	std::weak_ptr<ControllerComponent> m_controller;

};
