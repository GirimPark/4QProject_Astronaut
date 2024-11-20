#pragma once
#include "../Engine/GameObject.h"

class FPSCamera
	: public GameObject
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;
};

