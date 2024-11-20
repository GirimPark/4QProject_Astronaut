#include "pch.h"
#include "TestWorld.h"

#include "StaticTestObject.h"
#include "PlayerObject.h"
#include "TestPlayerObject.h"
#include "CubeMapObject.h"
#include "MapObject.h"
#include "BlockGenerator.h"
#include "GridManager.h"
#include "GridMovementComponent.h"

#include "../Engine/EventManager.h"
#include "../Engine/GameObject.h"
#include "../Engine/CameraComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/FPSCameraController.h"
#include "../Engine/Model.h"
#include "../Engine/WorldManager.h"
#include "../Engine/SoundManager.h"
#include "../Engine/UIText.h"
#include "../Engine/UIPanel.h"
#include "../Engine/UITriggerPopUp.h"
#include "../Engine/UIFunctorPopUp.h"
#include "../Engine/UIHoverPopUpFunctor.h"
#include "../Engine/UIClickPopUpFunctor.h"
#include "../Engine/RigidStaticComponent.h"

#include "ShockParticleObjcect.h"
#include "StaticFbxObject.h"
#include "SkeletalFbxObject.h"
#include "UIMeshTestObject.h"
#include "CreditObject.h"

#include "../NetworkLibrary/MyProtocol.h"
#include "../D3DRenderer/UIInstance.h"

#include <memory>
#include <string>


TestWorld::TestWorld()
{
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, this);
	//EventManager::GetInstance()->RegisterListener(eEventType::DELETE_OBJECT, this);

	WorldManager::GetInstance()->RegisterHandler(S2C_SET_TURN, std::bind(&TestWorld::SetTurn, this, std::placeholders::_1));
	WorldManager::GetInstance()->RegisterHandler(S2C_START_TURN, std::bind(&TestWorld::StartTurn, this, std::placeholders::_1));
	//WorldManager::GetInstance()->RegisterHandler(S2C_END_TURN, std::bind(&TestWorld::EndTurn, this, std::placeholders::_1));
	WorldManager::GetInstance()->RegisterHandler(S2C_CHANGE_TURN, std::bind(&TestWorld::ChangeTurn, this, std::placeholders::_1));
	WorldManager::GetInstance()->RegisterHandler(S2C_IS_ALL_READY, std::bind(&TestWorld::ClientsAllReady, this, std::placeholders::_1));
	//WorldManager::GetInstance()->RegisterHandler(S2C_CHARACTER_MOVE, std::bind(&TestWorld::CharacterMove, this, std::placeholders::_1));
}

TestWorld::~TestWorld()
{
	EventManager::GetInstance()->UnregisterListener(eEventType::CHANGE_WORLD, this);
	//EventManager::GetInstance()->UnregisterListener(eEventType::DELETE_OBJECT, this);
}

std::shared_ptr<TestPlayerObject> TestWorld::GetOtherPlayer(TestPlayerObject* player)
{
	if (player == m_hostPlayer.get())
		return m_guestPlayer;
	else
		return m_hostPlayer;

}

void TestWorld::Initialize()
{
	m_Camera = CreateGameObject<GameObject>("Test_Camera", eObjectType::CAMERA).lock();
	auto cameraComponent = m_Camera->CreateComponent<CameraComponent>("Test_Camera_Component");
	cameraComponent.lock()->SetLocalPosition({ 50.f, 100.f, -500.f });
	m_Camera->SetRootComponent(cameraComponent.lock());
	auto controllerComponent = m_Camera->CreateComponent<ControllerComponent>("Test_Controller_Component");
	controllerComponent.lock()->CreateController<FPSCameraController>();

	auto TestUI = m_UIManager->CreateUI<UIPanel>(L"TestUI");
	TestUI->SetTexturePath(L"../Resources/Textures/Snake.bmp");
	TestUI->SetSize({ 100.f, 100.f });
	TestUI->SetPosition({ -700.f, -300.f });
	TestUI->SetScale({ 1.5f, 1.5f });

	auto FadeInTestUI = m_UIManager->CreateUI<UIPanel>(L"FadeInTestUI");
	FadeInTestUI->SetSize({ 1920.f, 1080.f });
	FadeInTestUI->SetPosition({ 0.f, 0.f });
	FadeInTestUI->SetScale({ 1.0f, 1.0f });
	m_tFadeInOut = FadeInTestUI;

	//SoundManager::GetInstance()->LoadSound("../Resources/Sound/bgm.mp3", FMOD_LOOP_NORMAL);
	//SoundManager::GetInstance()->LoadSound("../Resources/Sound/jump.mp3");
	//SoundManager::GetInstance()->SetVolume("../Resources/Sound/jump.mp3", 1.5f);
	//SoundManager::GetInstance()->LoadSound("../Resources/Sound/effect.mp3");
	//SoundManager::GetInstance()->SetVolume("../Resources/Sound/effect.mp3", 0.1f);
	//SoundManager::GetInstance()->PlaySound("../Resources/Sound/bgm.mp3");
	//SoundManager::GetInstance()->SetVolume("../Resources/Sound/bgm.mp3", 0.1f);

	auto plane = CreateGameObject<GameObject>("Plane", eObjectType::PLANE).lock();
	auto planeRigidBody = plane->CreateComponent<RigidStaticComponent>("Plane_Rigidbody");
	auto planeSceneComponent = plane->CreateComponent<SceneComponent>("Plane_SceneComponent");
	plane->SetRootComponent(planeSceneComponent.lock());
	planeRigidBody.lock()->CreatePlane(0.f, 1.f, 0.f, 0.f, { 0.5f, 0.5f, 0.6f });

	m_hostPlayer = CreateGameObject<TestPlayerObject>("HostPlayer", eObjectType::PLAYER).lock();
	m_guestPlayer = CreateGameObject<TestPlayerObject>("GuestPlayer", eObjectType::PLAYER).lock();
	m_hostPlayer->SetHostPlayer(true);
	m_hostPlayer->SetTurn(true);

	auto TestMap = CreateGameObject<MapObject>("Map", eObjectType::MAP).lock();
	TestMap->SetFilePath("../Resources/FBX/map.fbx");

	auto shockParticleObjcect = CreateGameObject<ShockParticleObjcect>("Shock_Particle_Objcect", eObjectType::PARTICLE).lock();
	//m_guestPlayer = CreateGameObject<TestPlayerObject>("GuestPlayer", eObjectType::PLAYER).lock();

	  std::shared_ptr<StaticTestObject> TestObject = CreateGameObject<StaticTestObject>("TestObject", eObjectType::TEST).lock();
	std::shared_ptr<UIMeshTestObject> uiMeshObject = CreateGameObject<UIMeshTestObject>("UIMeshObject", eObjectType::TEST).lock();
	std::shared_ptr<CreditObject> endingCreditObject = CreateGameObject<CreditObject>("EndingCreditObject", eObjectType::TEST).lock();

	//if (WorldManager::GetInstance()->IsHostServer())
	//	m_hostPlayer->SetHostPlayer(true);
	//else m_guestPlayer->SetHostPlayer(true);


	// 재현
	//std::shared_ptr<StaticFbxObject> fbxTest = CreateGameObject<StaticFbxObject>("FBX_TEST2", eObjectType::TEST).lock();
	//std::shared_ptr<SkeletalFbxObject> fbxBearTest = CreateGameObject<SkeletalFbxObject>("FBX_TEST", eObjectType::TEST).lock();


	// CubeMap
	CreateGameObject<CubeMapObject>("CubeMap", eObjectType::TEST);

	// BlockGenerator
	std::shared_ptr<BlockGenerator> blockGenerator = std::make_shared<BlockGenerator>(shared_from_this(), "../Resources/Map/block_test2.yaml");
	blockGenerator->Generate();

	//CommonApp::m_pInstance->GetRenderer()->SetProjectionMatrix(cameraComponent.lock()->GetProjectionMatrix());

	LOG_CONTENTS("TestWorld1 Init");

	/*PacketC2S_SetTurn setTurn;
	setTurn.who = '0';
	WorldManager::GetInstance()->PushSendQueue(WorldManager::GetInstance()->SerializeBuffer(
		sizeof(PacketC2S_SetTurn), C2S_SET_TURN, &setTurn.who),
		sizeof(PacketC2S_SetTurn));*/

	__super::Initialize();

	/// --------------------------------------------

	/// GridManager
	m_gridManager = std::make_shared<GridManager>();
	m_gridManager->CreateMap(10, 10, 5, 0.f, 0.f, 0.f, 100.f);
	m_gridManager->SetMapState(GetGameObjects(eObjectType::LEVEL));
	m_gridManager->SetMapState(GetGameObjects(eObjectType::PLAYER));

	m_hostPlayer->SetGridManager(m_gridManager);
	m_guestPlayer->SetGridManager(m_gridManager);


	//TestObject->GetRootComponent().lock()->SetLocalPosition({ -100.f, 0.f, 0.f });
	//TestObject->GetRootComponent().lock()->SetLocalScale({ 1.f, 1.f, 1.f });


	// 재현
	//fbxTest->GetRootComponent().lock()->SetLocalPosition({ 150.f, 0.f, 50.f });
	//fbxTest->GetRootComponent().lock()->SetLocalScale({ 1.f, 1.f, 1.f });

	//fbxBearTest->GetRootComponent().lock()->SetLocalPosition({ -150.f, 0.f, -50.f });
	///fbxBearTest->GetRootComponent().lock()->SetLocalScale({ 1.f, 1.f, 1.f });
	//fbxBearTest->SetPosition(Vector3{ 100.f, 100.f, 100.f });


	// 건재 : UI 애니메이션 테스트
	{
		for (int i = 0; i < 13; i++)
		{
			CB_UIAnimationKeyframe keyframe;
			keyframe.m_Offset.x = 100.f * i;
			keyframe.m_Offset.y = 400.f;
			keyframe.m_TextureSize.x = 1300.f;
			keyframe.m_TextureSize.y = 600.f;
			keyframe.m_Size.x = 100.f;
			keyframe.m_Size.y = 100.f;
			keyframe.m_AnimationTime = 0.2f;
			TestUI->GetUIInstance().lock()->SetIsLoop(false);
			TestUI->AddKeyFrame(keyframe);
		}

		//
		FadeInTestUI->GetUIInstance().lock()->SetIsFade();
		FadeInTestUI->GetUIInstance().lock()->SetFadeDurationTime(1.5f);
	}

	m_bGameRun = false;
	m_bTurn = false;
	m_fixedDelta = 0.0f;
}

void TestWorld::Update(float deltaTime)
{
	World::Update(deltaTime);

	if (GetAsyncKeyState(VK_F2) & 0x8000)
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::TEST2);

	if (GetAsyncKeyState(VK_F3) & 0x8000)
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::TEST3);

	if (m_tFadeInOut != nullptr)
	{
		m_tFadeInOut->GetUIInstance().lock()->UpdateFadeInOut(deltaTime);
	}

	// 모든 클라가 레디라면!
	if (m_bGameRun)
	{
		m_fixedDelta += deltaTime;
		if (m_fixedDelta > 5.f)
		{
			EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::TEST2);
		}

	}
}

// 턴 설정해주기
void TestWorld::SetTurn(char* pData)
{
	if (pData[4] == '0')
	{
		m_bTurn = true;
		printf("1P 턴이당\n");
	}
	else if (pData[4] == '1')
	{
		m_bTurn = true;
		printf("2P 턴이당\n");
	}

	delete[] pData;
}

void TestWorld::StartTurn(char* pData)
{
	delete[] pData;
	// 누구 턴인지 BroadCast하게 해줌.
	char* broadCastTurn = new char[SND_BUF_SIZE];
	const char* msg = "1P Turn";
	memcpy(broadCastTurn, msg, SND_BUF_SIZE);

	WorldManager::GetInstance()->PushSendQueue(WorldManager::GetInstance()->SerializeBuffer(
		sizeof(PacketC2S_BroadcastMsg), C2S_BROADCAST_MSG, broadCastTurn),
		sizeof(PacketC2S_BroadcastMsg));
}

void TestWorld::EndTurn(char* pData)
{
	if (m_bTurn)
		m_bTurn = false;

	delete[] pData;
}

void TestWorld::ChangeTurn(char* pData)
{
	m_bTurn = m_bTurn ? false : true;

	delete[] pData;
}

void TestWorld::ClientsAllReady(char* pData)
{
	PacketS2C_IsAllReady* pAllReady = reinterpret_cast<PacketS2C_IsAllReady*>(pData);
	assert(pAllReady != nullptr);

	m_bGameRun = true;

	if (m_bGameRun)
	{
		auto ui = m_UIManager->GetUIObject<UITriggerPopUp>(L"UITestPopUp");
		//ui.lock()->TriggerPop();
	}
	delete[] pData;
}

void TestWorld::CharacterMove(char* pData)
{
	//PacketS2C_CharacterMove* pCharacterMove = reinterpret_cast<PacketS2C_CharacterMove*>(pData);
	//assert(pCharacterMove != nullptr);

	//for (auto& object : m_gameObjects[static_cast<int>(eObjectType::PLAYER)])
	//{
	//	std::shared_ptr<TestPlayerObject> pObj = std::dynamic_pointer_cast<TestPlayerObject>(object);
	//	if (pObj != nullptr)
	//	{
	//		// 호스트 서버에서 플레이어가 움직였다
	//		if (pCharacterMove->who == '0')
	//		{
	//			if (WorldManager::GetInstance()->IsHostServer() && pObj->GetHostPlayer())
	//			{
	//				// z축 이동
	//				if (pCharacterMove->direction == '0')
	//				{
	//					Math::Vector3 pos = pObj->GetRootComponent().lock()->GetLocalPosition();
	//					pObj->GetRootComponent().lock()->SetLocalPosition({ pos.x, pos.y, pos.z + 50.f });
	//				}
	//			}
	//			else if (!WorldManager::GetInstance()->IsHostServer() && !pObj->GetHostPlayer())
	//			{
	//				// z축 이동
	//				if (pCharacterMove->direction == '0')
	//				{
	//					Math::Vector3 pos = pObj->GetRootComponent().lock()->GetLocalPosition();
	//					pObj->GetRootComponent().lock()->SetLocalPosition({ pos.x, pos.y, pos.z + 50.f });
	//				}
	//			}
	//		}
	//		// 게스트 서버에서 플레이어가 움직였다
	//		else
	//		{
	//			if (!WorldManager::GetInstance()->IsHostServer() && pObj->GetHostPlayer())
	//			{
	//				// z축 이동
	//				if (pCharacterMove->direction == '0')
	//				{
	//					Math::Vector3 pos = pObj->GetRootComponent().lock()->GetLocalPosition();
	//					pObj->GetRootComponent().lock()->SetLocalPosition({ pos.x, pos.y, pos.z + 50.f });
	//				}
	//			}
	//			else if (WorldManager::GetInstance()->IsHostServer() && !pObj->GetHostPlayer())
	//			{
	//				// z축 이동
	//				if (pCharacterMove->direction == '0')
	//				{
	//					Math::Vector3 pos = pObj->GetRootComponent().lock()->GetLocalPosition();
	//					pObj->GetRootComponent().lock()->SetLocalPosition({ pos.x, pos.y, pos.z + 50.f });
	//				}
	//			}
	//		}
	//	}
	//}
	delete[] pData;
}

void TestWorld::OnEnter()
{
	InputManager::GetInstance()->AddInputProcesser(this);
	World::OnEnter();
}

void TestWorld::OnExit()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);

	World::OnExit();
}

void TestWorld::HandleEvent(Event* event)
{
	switch (event->eventID)
	{
	case eEventType::CHANGE_WORLD:
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
		break;
	case eEventType::DELETE_OBJECT:
		// 해당 타입에서 방금 생성한 오브젝트 삭제하는 테스트 코드
		//if (m_gameObjects[static_cast<int>(event->GetData<eObjectType>())].size() <= 0)
		//	break;
		//std::string objName = m_gameObjects[static_cast<int>(event->GetData<eObjectType>())][m_gameObjects[static_cast<int>(event->GetData<eObjectType>())].size() - 1]->GetName();
		//if (!objName.empty())
		//	DeleteGameObject(objName);
		break;
	}
}

void TestWorld::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
	const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	if (m_bTurn)
	{
		if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::Up))
		{
			CreateGameObjectRuntime<StaticTestObject>("StaticMesh", eObjectType::PLAYER);
			CreateGameObjectRuntime<PlayerObject>("SkeletalMesh", eObjectType::PLAYER);
		}
		if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::Down))
		{
			// 해당 타입에서 방금 생성한 오브젝트 삭제하는 테스트 코드
			//EventManager::GetInstance()->SendEvent(eEventType::DELETE_OBJECT, this, eObjectType::PLAYER);
			//EventManager::GetInstance()->SendEvent(eEventType::DELETE_OBJECT, this, eObjectType::PLAYER);
		}

		if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::Enter))
		{
			// 턴 넘겨주기
			WorldManager::GetInstance()->PushSendQueue(
				WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_ChangeTurn), C2S_CHANGE_TURN, nullptr),
				sizeof(PacketC2S_ChangeTurn));
		}
	}


	// 재현 : 페이드 인 아웃 테스트
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::Z))
	{
		m_tFadeInOut->GetUIInstance().lock()->FadeOutStart();
	}
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::X))
	{
		m_tFadeInOut->GetUIInstance().lock()->FadeInStart();
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::Delete))
	{
		// 준비 버튼
		PacketC2S_READY ready = {};
		ready.clickedReady = '1';

		WorldManager::GetInstance()->PushSendQueue(
			WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_READY), C2S_READY, &ready.clickedReady),
			sizeof(PacketC2S_READY));
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		SoundManager::GetInstance()->PlaySound("../Resources/Sound/jump.mp3");
	}

	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::V))
	{
		SoundManager::GetInstance()->PlaySound("../Resources/Sound/effect.mp3");
	}
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::B))				// 머터리얼 바뀌는 거 테스트용 코드
	{
		weak_ptr<UIMeshTestObject> object = dynamic_pointer_cast<UIMeshTestObject>(GetGameObject("UIMeshObject"));
		object.lock()->ChangedMaterial();
	}
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::N))				// 머터리얼 바뀌는 거 테스트용 코드
	{
		weak_ptr<UIMeshTestObject> object = dynamic_pointer_cast<UIMeshTestObject>(GetGameObject("UIMeshObject"));
		object.lock()->PlayParticle();
		object.lock()->ChangedUIMaterial();
		object.lock()->ChangedMeshOutLine();
	}

}
