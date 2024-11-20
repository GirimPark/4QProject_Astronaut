#include "pch.h"
#include "SkeletalTestObject.h"

#include "../Engine/SkeletalMeshComponent.h"

SkeletalTestObject::~SkeletalTestObject()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);
}

void SkeletalTestObject::Initialize()
{
	InputManager::GetInstance()->AddInputProcesser(this);

	m_meshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent");
	m_meshComponent.lock()->SetDefaultAnimation("../Resources/FBX/Zombie_Run.fbx");
	m_meshComponent.lock()->AddAnimation("../Resources/FBX/SkinningTest.fbx");
	m_meshComponent.lock()->SetLocalPosition({ 100.f, 0.f, 0.f });
	SetRootComponent(m_meshComponent.lock());


	GameObject::Initialize();
}

void SkeletalTestObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void SkeletalTestObject::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::N))
	{
		m_meshComponent.lock()->ChangeAnimation("../Resources/FBX/Zombie_Run.fbx");
	}
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::M))
	{
		m_meshComponent.lock()->ChangeAnimation("../Resources/FBX/SkinningTest.fbx");
	}
}
