#include "pch.h"
#include "EndingCreditWorld.h"

#include "EndingCreditObject.h"
#include "CubeMapObject.h"

#include "../Engine/CameraComponent.h"
#include "../Engine/GameObject.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/FPSCameraController.h"

EndingCreditWorld::EndingCreditWorld()
{

}

EndingCreditWorld::~EndingCreditWorld()
{

}

void EndingCreditWorld::Initialize()
{
	LOG_CONTENTS("EndingCreditWorld Init");

	m_Camera = CreateGameObject<GameObject>("Test_Camera", eObjectType::CAMERA).lock();
	auto cameraComponent = m_Camera->CreateComponent<CameraComponent>("Test_Camera_Component");
	cameraComponent.lock()->SetLocalPosition({ 0.f, 0.f, -700.f });
	m_Camera->SetRootComponent(cameraComponent.lock());
	auto controllerComponent = m_Camera->CreateComponent<ControllerComponent>("Test_Controller_Component");
	controllerComponent.lock()->CreateController<FPSCameraController>();

	std::shared_ptr<EndingCreditObject> endingCreditObject = CreateGameObject<EndingCreditObject>("EndingCreditObject", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject1 = CreateGameObject<EndingCreditObject>("EndingCreditObject1", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject2 = CreateGameObject<EndingCreditObject>("EndingCreditObject2", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject3 = CreateGameObject<EndingCreditObject>("EndingCreditObject3", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject4 = CreateGameObject<EndingCreditObject>("EndingCreditObject4", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject5 = CreateGameObject<EndingCreditObject>("EndingCreditObject5", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject6 = CreateGameObject<EndingCreditObject>("EndingCreditObject6", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject7 = CreateGameObject<EndingCreditObject>("EndingCreditObject7", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject8 = CreateGameObject<EndingCreditObject>("EndingCreditObject8", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject9 = CreateGameObject<EndingCreditObject>("EndingCreditObject9", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject10 = CreateGameObject<EndingCreditObject>("EndingCreditObject10", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject11 = CreateGameObject<EndingCreditObject>("EndingCreditObject11", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject12 = CreateGameObject<EndingCreditObject>("EndingCreditObject12", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject13 = CreateGameObject<EndingCreditObject>("EndingCreditObject13", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject14 = CreateGameObject<EndingCreditObject>("EndingCreditObject14", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject15 = CreateGameObject<EndingCreditObject>("EndingCreditObject15", eObjectType::TEST).lock();
	std::shared_ptr<EndingCreditObject> endingCreditObject16 = CreateGameObject<EndingCreditObject>("EndingCreditObject16", eObjectType::TEST).lock();

	auto cube = CreateGameObject<CubeMapObject>("CubeMap", eObjectType::TEST);

	__super::Initialize();

	cube.lock()->SetScale(Vector3(500.f, 500.f, 500.f));
	endingCreditObject->SetPosition(Vector3(1500.f, 0.f, 0.f));
	endingCreditObject1->SetPosition(Vector3(6000.f, 3000.f, 10000.f));
	endingCreditObject2->SetPosition(Vector3(100.f, -4000.f, 14000.f));
	endingCreditObject3->SetPosition(Vector3(1200.f, -2000.f, 15000.f));
	endingCreditObject4->SetPosition(Vector3(4000.f, 3000.f, 20000.f));
	endingCreditObject5->SetPosition(Vector3(3000.f, -6000.f, 250000.f));
	endingCreditObject6->SetPosition(Vector3(100.f, 4000.f, 3000.f));
	endingCreditObject7->SetPosition(Vector3(3000.f, -1500.f, 7500.f));
	endingCreditObject8->SetPosition(Vector3(8000.f, -3500.f, 8000.f));
	endingCreditObject9->SetPosition(Vector3(5000.f, -2000.f, 30000.f));
	endingCreditObject10->SetPosition(Vector3(-500.f, -1500.f, 7500.f));
	endingCreditObject11->SetPosition(Vector3(-1000.f, -3500.f, 8000.f));
	endingCreditObject12->SetPosition(Vector3(1000.f, 6000.f, 18000.f));
	endingCreditObject13->SetPosition(Vector3(9000.f, -1500.f, 13000.f));
	endingCreditObject14->SetPosition(Vector3(12000.f, -3500.f, 10000.f));
	endingCreditObject15->SetPosition(Vector3(15000.f, 2000.f, 7000.f));
	endingCreditObject16->SetPosition(Vector3(12000.f, 600.f, 9000.f));
}

void EndingCreditWorld::Update(float deltaTime)
{
	World::Update(deltaTime);
}

void EndingCreditWorld::OnEnter()
{
	World::OnEnter();
}

void EndingCreditWorld::OnExit()
{
	World::OnExit();
}

void EndingCreditWorld::HandleEvent(Event* event)
{

}
