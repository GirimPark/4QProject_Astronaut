#pragma once
#include "../Engine/GameObject.h"

class EnvironmentComponent;

class CubeMapObject
	: public GameObject
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::shared_ptr<EnvironmentComponent> m_environmentComponent;
};

