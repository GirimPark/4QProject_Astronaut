#include "pch.h"
#include "CreditScene.h"

#include "CreditObject.h"
#include "CubeMapObject.h"

#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"
#include "../Engine/SoundManager.h"
#include "../Engine/CameraComponent.h"
#include "../Engine/FPSCameraController.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/UIObject.h"
#include "PlanetObject.h"

void CreditScene::Initialize()
{
	LOG_CONTENTS("CreditScene Init");

	SoundManager::GetInstance()->StopAllSound();
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/BGM/BGM_Scene_ED.mp3", FMOD_LOOP_NORMAL);
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/BGM/BGM_Scene_ED.mp3");
	
	m_Camera = CreateGameObject<GameObject>("Test_Camera", eObjectType::CAMERA).lock();
	auto cameraComponent = m_Camera->CreateComponent<CameraComponent>("Test_Camera_Component");
	cameraComponent.lock()->SetLocalPosition({ 0.f, 0.f, -700.f });
	m_Camera->SetRootComponent(cameraComponent.lock());
	//auto controllerComponent = m_Camera->CreateComponent<ControllerComponent>("Test_Controller_Component");
	//controllerComponent.lock()->CreateController<FPSCameraController>();

	std::shared_ptr<CreditObject> creditObject = CreateGameObject<CreditObject>("EndingCreditObject", eObjectType::TEST).lock();
	std::shared_ptr<PlanetObject> planetObject1 = CreateGameObject<PlanetObject>("PlanetObject", eObjectType::TEST).lock();
	std::shared_ptr<PlanetObject> planetObject2 = CreateGameObject<PlanetObject>("PlanetObject", eObjectType::TEST).lock();
	std::shared_ptr<PlanetObject> planetObject3 = CreateGameObject<PlanetObject>("PlanetObject", eObjectType::TEST).lock();
	std::shared_ptr<PlanetObject> planetObject4 = CreateGameObject<PlanetObject>("PlanetObject", eObjectType::TEST).lock();

	planetObject1->SetPath("../Resources/FBX/earth.fbx");
	planetObject2->SetPath("../Resources/FBX/midoricloud.fbx");
	planetObject3->SetPath("../Resources/FBX/pinkmilk.fbx");
	planetObject4->SetPath("../Resources/FBX/yellowring.fbx");
	
	m_CreditUI = m_UIManager->CreateUI<UIObject>(L"CreditNameTextTexture");
	m_CreditUI->SetTexturePath(L"../Resources/Textures/credit Text.png");
	m_CreditUI->SetSize({4080.f, 170.f});
	m_CreditUI->SetPosition({5700.f, 0.f});
	m_CreditUI->SetVisible(true);

	// 큐브맵
	auto cube = CreateGameObject<CubeMapObject>("CubeMap", eObjectType::TEST);

	__super::Initialize();

	cube.lock()->SetScale(Vector3(500.f, 500.f, 500.f));
	creditObject->SetPosition(Vector3(1500.f, 0.f, 0.f));
	planetObject1->SetPosition(Vector3(1500.f, 1500.f, 4000.f));
	planetObject2->SetPosition(Vector3(3500.f, -1800.f, 8000.f));
	planetObject3->SetPosition(Vector3(-3000.f, 1500.f, 10000.f));
	planetObject4->SetPosition(Vector3(-2000.f, -1500.f, 5000.f));
}

void CreditScene::Update(float deltaTime)
{
	World::Update(deltaTime);

	Vector2 uiPosition = m_CreditUI->GetPosition();
	uiPosition += Vector2( -240.f * deltaTime, 0.f * deltaTime );
	m_CreditUI->SetPosition(uiPosition);
}

void CreditScene::OnEnter()
{
	InputManager::GetInstance()->AddInputProcesser(this);

	World::OnEnter();
}

void CreditScene::OnExit()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);

	World::OnExit();
}

void CreditScene::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}

void CreditScene::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
	const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::MAIN);
	}

	// 치트키
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F1))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STAGE1);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F2))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STAGE2);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F3))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STAGE3);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F4))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STAGE4);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F5))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STAGE5);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F6))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STARTCUTSCECNE);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F7))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STARTFREEWORLD);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F8))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::ENDCUTSCENE);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F9))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::ENDFREEWORLD);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F10))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::CREDIT);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F11))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::GAMEOVERWORLD);
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::F11))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::MAIN);
	}
}


//std::shared_ptr<CreditObject> creditObject1 = CreateGameObject<CreditObject>("EndingCreditObject1", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject2 = CreateGameObject<CreditObject>("EndingCreditObject2", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject3 = CreateGameObject<CreditObject>("EndingCreditObject3", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject4 = CreateGameObject<CreditObject>("EndingCreditObject4", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject5 = CreateGameObject<CreditObject>("EndingCreditObject5", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject6 = CreateGameObject<CreditObject>("EndingCreditObject6", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject7 = CreateGameObject<CreditObject>("EndingCreditObject7", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject8 = CreateGameObject<CreditObject>("EndingCreditObject8", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject9 = CreateGameObject<CreditObject>("EndingCreditObject9", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject10 = CreateGameObject<CreditObject>("EndingCreditObject10", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject11 = CreateGameObject<CreditObject>("EndingCreditObject11", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject12 = CreateGameObject<CreditObject>("EndingCreditObject12", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject13 = CreateGameObject<CreditObject>("EndingCreditObject13", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject14 = CreateGameObject<CreditObject>("EndingCreditObject14", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject15 = CreateGameObject<CreditObject>("EndingCreditObject15", eObjectType::TEST).lock();
//std::shared_ptr<CreditObject> creditObject16 = CreateGameObject<CreditObject>("EndingCreditObject16", eObjectType::TEST).lock();
//creditObject1->SetPosition(Vector3(6000.f, 3000.f, 10000.f));
//creditObject2->SetPosition(Vector3(100.f, -4000.f, 14000.f));
//creditObject3->SetPosition(Vector3(1200.f, -2000.f, 15000.f));
//creditObject4->SetPosition(Vector3(4000.f, 3000.f, 20000.f));
//creditObject5->SetPosition(Vector3(3000.f, -6000.f, 250000.f));
//creditObject6->SetPosition(Vector3(100.f, 4000.f, 3000.f));
//creditObject7->SetPosition(Vector3(3000.f, -1500.f, 7500.f));
//creditObject8->SetPosition(Vector3(8000.f, -3500.f, 8000.f));
//creditObject9->SetPosition(Vector3(5000.f, -2000.f, 30000.f));
//creditObject10->SetPosition(Vector3(-500.f, -1500.f, 7500.f));
//creditObject11->SetPosition(Vector3(-1000.f, -3500.f, 8000.f));
//creditObject12->SetPosition(Vector3(1000.f, 6000.f, 18000.f));
//creditObject13->SetPosition(Vector3(9000.f, -1500.f, 13000.f));
//creditObject14->SetPosition(Vector3(12000.f, -3500.f, 10000.f));
//creditObject15->SetPosition(Vector3(15000.f, 2000.f, 7000.f));
//creditObject16->SetPosition(Vector3(12000.f, 600.f, 9000.f));