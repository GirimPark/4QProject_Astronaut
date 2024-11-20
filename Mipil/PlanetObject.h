#pragma once
#include "../Engine/GameObject.h"

class StaticMeshComponent;

class PlanetObject : public GameObject
{
public:
	void SetPath(std::string_view path) { m_Path = path.data(); }

public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::shared_ptr<StaticMeshComponent> m_pMeshComponent;

	std::string m_Path;
	float m_Rotation;
};

