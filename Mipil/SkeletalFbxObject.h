#pragma once
#include "../Engine/GameObject.h"

class SkeletalMeshComponent;

class SkeletalFbxObject
	: public GameObject
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

	void SetFbxObject(std::string filePath);

private:
	std::weak_ptr<SkeletalMeshComponent> m_meshComponent;
	std::string m_filePath = "../Resources/FBX/bear_Idle.fbx"; 
};

