#include "pch.h"
#include "SkeletalFbxObject.h"

#include "../Engine/SkeletalMeshComponent.h"
#include "PointLightComponent.h"

void SkeletalFbxObject::Initialize()
{
	m_meshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent");
 	m_meshComponent.lock()->SetDefaultAnimation(m_filePath);
 //	m_meshComponent.lock()->SetDefaultAnimation("../Resources/FBX/bear_Idle.fbx");
	//m_meshComponent.lock()->AddAnimation("../Resources/FBX/spaceship_ani.fbx", false);

	SetRootComponent(m_meshComponent.lock());
 
	//auto pointLight = CreateComponent<PointLightComponent>("Test_PointLight1").lock();
	//pointLight->SetRange(100);
	//pointLight->SetPointLightIntencity(0.1f);
	//pointLight->SetLightColor(0 , 64 , 32); // R G B

	GameObject::Initialize();
}

void SkeletalFbxObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void SkeletalFbxObject::SetFbxObject(std::string filePath)
{
	m_filePath = filePath;
}
