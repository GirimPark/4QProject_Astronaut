#include "pch.h"
#include "TestWorld3.h"
#include "../Engine/WorldManager.h"
#include "../Engine/EventManager.h"

#include "../Engine/GameObject.h"

#include "../Engine/CameraComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/FPSCameraController.h"

#include "../Engine/RigidStaticComponent.h"
#include "../Engine/RigidDynamicComponent.h"

#include "TestPlayerController.h"
#include "TestCollisionHandler.h"


void TestWorld3::Initialize()
{
	LOG_CONTENTS("TestWorld3 Init");

	m_Camera = CreateGameObject<GameObject>("Test_Camera", eObjectType::CAMERA).lock();
	auto cameraComponent = m_Camera->CreateComponent<CameraComponent>("Test_Camera_Component");
	cameraComponent.lock()->SetLocalPosition({ 50.f, 100.f, -500.f });
	m_Camera->SetRootComponent(cameraComponent.lock());
	auto controllerComponent = m_Camera->CreateComponent<ControllerComponent>("Test_Controller_Component");
	controllerComponent.lock()->CreateController<FPSCameraController>();

	m_plane = CreateGameObject<GameObject>("Plane", eObjectType::PLANE).lock();
	auto planeRigidBody = m_plane->CreateComponent<RigidStaticComponent>("Plane_Rigidbody");
	auto planeSceneComponent = m_plane->CreateComponent<SceneComponent>("Plane_SceneComponent");
	m_plane->SetRootComponent(planeSceneComponent.lock());
	planeRigidBody.lock()->CreatePlane(0.f, 1.f, 0.f, 0.f, { 0.5f, 0.5f, 0.6f });

	m_staticActor = CreateGameObject<GameObject>("Static_Actor", eObjectType::LEVEL).lock();
	auto BoxRigidBody = m_staticActor->CreateComponent<RigidStaticComponent>("Box_Rigidbody");
	auto BoxSceneComponent = m_staticActor->CreateComponent<SceneComponent>("Box_SceneComponent");
	BoxSceneComponent.lock()->SetLocalPosition({ 50.f, 10.f, 0.f });
	m_staticActor->SetRootComponent(BoxSceneComponent.lock());
	BoxRigidBody.lock()->CreateStaticRigidBody(RigidBodyComponent::Geometry::Box, { 10, 10, 10 }, { 0.5f, 0.5f, 0.6f });
	BoxRigidBody.lock()->AddTriggerShape(RigidBodyComponent::Geometry::Sphere, { 30.f }, { 0.f, 20.f, 0.f });

	m_dynamicActor = CreateGameObject<GameObject>("Dynamic_Actor", eObjectType::PLAYER).lock();
	auto CapsuleRigidBody = m_dynamicActor->CreateComponent<RigidDynamicComponent>("Capsule_Rigidbody");
	auto CapsuleSceneComponent = m_dynamicActor->CreateComponent<SceneComponent>("Capsule_SceneComponent");
	CapsuleSceneComponent.lock()->SetLocalPosition({ -50.f, 10.f, 0.f });
	CapsuleSceneComponent.lock()->SetLocalRotation({ 0.f, 0.f, 90.f });
	m_dynamicActor->SetRootComponent(CapsuleSceneComponent.lock());
	CapsuleRigidBody.lock()->CreateDynamicRigidBody(RigidBodyComponent::Geometry::Capsule, {5.f, 10.f}, { 0.5f, 0.5f, 0.6f }, 0.2f);
	CapsuleRigidBody.lock()->SetLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	CapsuleRigidBody.lock()->AddCollisionHandler<TestCollisionHandler>();
	controllerComponent = m_dynamicActor->CreateComponent<ControllerComponent>("Test_Player_Controller_Component");
	controllerComponent.lock()->CreateController<TestPlayerController>();

	__super::Initialize();
}

void TestWorld3::Update(float deltaTime)
{
	World::Update(deltaTime);

	if (GetAsyncKeyState(VK_F1) & 0x8000)
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::TEST);

	if (GetAsyncKeyState(VK_F2) & 0x8000)
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::TEST2);
}

void TestWorld3::OnEnter()
{
	World::OnEnter();
}

void TestWorld3::OnExit()
{
	World::OnExit();
}

void TestWorld3::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
}

void TestWorld3::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}
