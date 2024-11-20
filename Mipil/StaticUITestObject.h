#pragma once
#include "../Engine/GameObject.h"

class StaticMeshComponent;

class StaticUITestObject :
    public GameObject
{
public:
    void Initialize() override;
    void Update(float deltaTime) override;

public:


private:
    std::weak_ptr<StaticMeshComponent> m_MeshComponent;

};

