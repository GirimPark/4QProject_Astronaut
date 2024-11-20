#include "pch.h"
#include "StaticFbxObject.h"

#include "../Engine/StaticMeshComponent.h"
#include "../Engine/SkeletalMeshComponent.h"

void StaticFbxObject::Initialize()
{
	m_meshComponent = CreateComponent<StaticMeshComponent>("StaticMeshComponent");
	m_meshComponent.lock()->SetFilePath(m_filePath);
	//m_meshComponent.lock()->SetFilePath("../Resources/FBX/brokenroket_ingame_low.fbx");
	//m_meshComponent.lock()->SetFilePath("../Resources/FBX/zeldaPosed001.fbx");
 	SetRootComponent(m_meshComponent.lock());

	GameObject::Initialize();

	m_meshComponent.lock()->SetLocalPosition(m_deferredPosition);
}

void StaticFbxObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void StaticFbxObject::SetFilePath(std::string filePath)
{
	m_filePath = filePath;
}
