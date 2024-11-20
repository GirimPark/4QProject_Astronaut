#include "pch.h"
#include "EndingFreeWorld.h"

#include "CubeMapObject.h"
#include "SkeletalFbxObject.h"
#include "MapObject.h"
#include "StaticFbxObject.h"
#include "PlayerFreeCamera.h"
#include "PlayerFreeObject.h"
#include "ClearCollisionHandler.h"
#include "FireParticleObject.h"

#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"
#include "../Engine/UIPanel.h"
#include "../Engine/UIText.h"
#include "../Engine/RigidStaticComponent.h"
#include "../Engine/SceneComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/RigidDynamicComponent.h"
#include "../Engine/SkeletalMeshComponent.h"

#include <sstream>   // std::ostringstream
#include <iomanip>   // std::setprecision
#include <cmath>
#include <locale>
#include <memory>

//		m_tFadeInOut->GetUIInstance().lock()->FadeInStart();
//		m_dimUI->GetUIInstance().lock()->SetUseDim(true);

void EndingFreeWorld::Initialize()
{
	LOG_CONTENTS("EndingFreeWorld Init");

	WorldManager::GetInstance()->RegisterHandler(S2C_IS_ALL_END, std::bind(&EndingFreeWorld::GotoEndingCutScene, this, std::placeholders::_1));

	// CubeMap
	CreateGameObject<CubeMapObject>("CubeMap", eObjectType::TEST);

	m_player = CreateGameObject<PlayerFreeObject>("Player", eObjectType::PLAYER).lock();
	m_player->SetDeferredPosition({ -219.f, 100.f, -4840 });
	m_player->SetDeferredRotation({ 0.f, -165.2f, 0.f });

	auto fire = CreateGameObject<FireParticleObject>("fire", eObjectType::PARTICLE).lock();
	fire->SetParentObject(m_player);
	m_player->SetFireParticle(fire);

	// Camera //
	m_mainCamera = CreateGameObject<PlayerFreeCamera>("mainCamera", eObjectType::CAMERA).lock();
	m_mainCamera->SetSubject(m_player);
	m_player->SetCamera(m_mainCamera.get());

	// Map //
	auto collisionMap = CreateGameObject<MapObject>("collisionMap", eObjectType::MAP).lock(); // 필드
	collisionMap->SetFilePath("../Resources/FBX/freeMap_collsion_obj.fbx");
	//collisionMap->SetFilePath("../Resources/FBX/freeMap_collsion_update.fbx");

	auto decoMap = CreateGameObject<StaticFbxObject>("decoMap", eObjectType::MAP).lock(); // 데코
	decoMap->SetFilePath("../Resources/FBX/freeMap_decor_obj.fbx");

	auto stage = CreateGameObject<MapObject>("stage", eObjectType::MAP).lock(); //단상
	stage->SetFilePath("../Resources/FBX/freemap_Stage.fbx");
	 
	auto clearTrigger = CreateGameObject<GameObject>("ClearTrigger", eObjectType::READYTRIGGER).lock();
	auto triggerRigidBody = clearTrigger->CreateComponent<RigidStaticComponent>("Trigger_RigidBody").lock();
	auto triggerSceneComponent = clearTrigger->CreateComponent<SceneComponent>("Trigger_SceneComponent").lock();
	triggerSceneComponent->SetLocalPosition({ -1998.f, 100.f, 5492.f });
	triggerSceneComponent->SetLocalRotation({ 0.f, 171.f, 0.f });
	clearTrigger->SetRootComponent(triggerSceneComponent);
	triggerRigidBody->CreateStaticRigidBody(RigidBodyComponent::Geometry::Box, { 2000, 10, 10 }, { 0.5f, 0.5f, 0.6f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, true);
	triggerRigidBody->AddCollisionHandler<ClearCollisionHandler>();

	// Chat Background
	{
		auto chat = m_UIManager->CreateUI<UIPanel>(L"chat");
		chat->SetTexturePath(L"../Resources/Textures/FreeMove_bg.png");
		chat->SetSize({ 1920.f, 1080.f });
		chat->SetPosition({ 0.f, 0.f });
		chat->SetScale({ 1.f, 1.f });

		float m_distanceTriggerArray[] = { 5, 15, 50, 60 };
		SetDialogue(m_distanceTriggerArray);
	}

	// dim
	auto dimUI = m_UIManager->CreateUI<UIPanel>(L"dimUI");
	dimUI->SetSize({ 1920.f, 1080.f });
	dimUI->SetPosition({ 0.f, 0.f });
	dimUI->SetScale({ 1.0f, 1.0f });
	m_dimUI = dimUI;

	m_DistanceText = m_UIManager->CreateUI<UIText>(L"m_DistanceText");
	m_DistanceText->SetPosition({ 0.f, 450.f });
	m_DistanceText->SetFont(L"KoPubWold돋움체 Bold");
	m_DistanceText->SetFontSize(30.f);
	m_DistanceText->SetColor(D2D1::ColorF::White);
	m_DistanceText->SetText(std::to_wstring(m_remainDistance));

	// FadeInOut
	auto FadeInTestUI = m_UIManager->CreateUI<UIPanel>(L"FadeInTestUI");
	FadeInTestUI->SetSize({ 1920.f, 1080.f });
	FadeInTestUI->SetPosition({ 0.f, 0.f });
	FadeInTestUI->SetScale({ 1.0f, 1.0f });
	m_tFadeInOut = FadeInTestUI;

	// 로딩 스프라이트 애니메이션
	auto LoadingPanelUI = m_UIManager->CreateUI<UIPanel>(L"LoadingPanelUI");
	LoadingPanelUI->SetTexturePath(L"../Resources/Textures/Over_Anim_Waiting for call.png");
	LoadingPanelUI->SetSize({ 684.f, 128.f });
	LoadingPanelUI->SetPosition({ 0.f,  92.f });
	LoadingPanelUI->SetScale({ 1.f, 1.f });
	LoadingPanelUI->SetVisible(false);

	__super::Initialize();

	fire->SetLocalPositions({ 0.f, -160.f, -40.f });

	// Distance
	{
		m_totlaDistance = CalcDistance(m_player->GetWorldPosition(), m_destPosition);
		m_destPosition = { -1998.f, 100.f, 5492.f };
	}

	// FadeInOut
	FadeInTestUI->GetUIInstance().lock()->SetIsFade();
	FadeInTestUI->GetUIInstance().lock()->SetFadeDurationTime(1.5f);
	m_tFadeInOut->GetUIInstance().lock()->FadeInStart();

	for (int i = 0; i < 4; i++)
	{
		CB_UIAnimationKeyframe keyframe;
		keyframe.m_Offset.x = 684.f * i;
		keyframe.m_Offset.y = 0.f;
		keyframe.m_TextureSize.x = 2736.f;
		keyframe.m_TextureSize.y = 128.f;
		keyframe.m_Size.x = 684.f;
		keyframe.m_Size.y = 128.f;
		keyframe.m_AnimationTime = 0.5f;
		LoadingPanelUI->AddKeyFrame(keyframe);
	}
	LoadingPanelUI->GetUIInstance().lock()->SetIsLoop(true);

	//
	m_dimUI->GetUIInstance().lock()->SetIsDim();

	m_playerRigid = m_player->GetComponentByTypeName<RigidDynamicComponent>().lock().get();
	m_playerMesh = m_player->GetComponentByTypeName<SkeletalMeshComponent>().lock().get();
}

float EndingFreeWorld::CalcDistance(Math::Vector3 startPos, Math::Vector3 destPos)
{
	// 높이는 거리에 상관없으므로 X,Z 축만으로 계산을 한다.
	float x = destPos.x - startPos.x;
	float z = destPos.z - startPos.z;
	float powXZ = pow(x, 2) + pow(z, 2);

	return std::sqrt(powXZ) * 0.01f;
}

void EndingFreeWorld::SetDialogue(float trigger[])
{
	float textPad = 31.0f;

	auto chatA = m_UIManager->CreateUI<UIPanel>(L"chatA");
	chatA->SetTexturePath(L"../Resources/ChatText/ending_a.png");
	chatA->SetSize({ 375.0f , 20.0f });
	chatA->SetPosition({ 720.f, 465.f });
	chatA->SetScale({ 1.f, 1.f });
	chatA->SetVisible(false);
	m_dialogueQueue.push({ chatA , trigger[0] });

	auto chatA2 = m_UIManager->CreateUI<UIPanel>(L"chatA2");
	chatA2->SetTexturePath(L"../Resources/ChatText/ending_b.png");
	chatA2->SetSize({ 375.0f , 20.0f });
	chatA2->SetPosition({ 720.f, 465.f - textPad });
	chatA2->SetScale({ 1.f, 1.f });
	chatA2->SetVisible(false);
	m_dialogueQueue.push({ chatA2, trigger[1] });

	auto chatC = m_UIManager->CreateUI<UIPanel>(L"chatC"); // 이미지 기준점 변경 필요
	chatC->SetTexturePath(L"../Resources/ChatText/ending_c.png");
	chatC->SetSize({ 375.0f , 20.0f });
	chatC->SetPosition({ 720.f, 465.f - textPad * 2 });
	chatC->SetScale({ 1.f, 1.f });
	chatC->SetVisible(false);
	m_dialogueQueue.push({ chatC , trigger[2] });

	auto chatD = m_UIManager->CreateUI<UIPanel>(L"chatD");
	chatD->SetTexturePath(L"../Resources/ChatText/ending_d.png");
	chatD->SetSize({ 375.0f , 20.0f });
	chatD->SetPosition({ 720.f, 465.f - textPad * 3 });
	chatD->SetScale({ 1.f, 1.f });
	chatD->SetVisible(false);
	m_dialogueQueue.push({ chatD, trigger[3] });
}

void EndingFreeWorld::GotoEndingCutScene(char* pMsg)
{
	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::ENDCUTSCENE);
	delete[] pMsg;
}

void EndingFreeWorld::TriggerDim(bool dim)
{
	m_isNextWorldTimer = true;
	m_dimUI->GetUIInstance().lock()->SetUseDim(dim);

	auto LoadingPanelUI = m_UIManager->GetUIObject<UIPanel>(L"LoadingPanelUI");
	LoadingPanelUI.lock()->SetVisible(true);
 }

void EndingFreeWorld::StopMove()
{
	m_playerRigid->GetRigidDynamic()->clearForce();
	m_playerRigid->GetRigidDynamic()->clearTorque();
	m_playerRigid->GetRigidDynamic()->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));
	m_playerRigid->GetRigidDynamic()->setAngularVelocity(PxVec3(0.f, 0.f, 0.f));
	m_playerRigid->GetRigidDynamic()->putToSleep();

	m_remainDistance = 0.f;
}

void EndingFreeWorld::DeletePlayerController()
{
	auto controllerCp = m_player->GetComponentByTypeName<ControllerComponent>().lock();
	if (controllerCp)
	{
		controllerCp->ClearControllers();
	}

	m_isTrigger = true;
}

void EndingFreeWorld::Update(float deltaTime)
{
	World::Update(deltaTime);

	UpdateAnimation();
	// Distance Chat UI
	{
		static bool isCalcedTotalDistance = false;
		if (isCalcedTotalDistance == false)
		{
			m_totlaDistance = CalcDistance(m_player->GetWorldPosition(), m_destPosition);
			isCalcedTotalDistance = true;
		}

		m_remainDistance = CalcDistance(m_player->GetWorldPosition(), m_destPosition);
		m_processDistance = m_totlaDistance - m_remainDistance;

		std::wstringstream wss;
		wss << std::fixed << std::setprecision(1) << m_remainDistance;
		m_DistanceText->SetText(wss.str());

		if (m_processDistance >= m_dialogueQueue.front().second)
		{
			if (m_dialogueQueue.size() > 0)
			{
				m_dialogueQueue.front().first->SetVisible(true);
				m_dialogueQueue.pop();
			}
		}
	}

	if (m_isNextWorldTimer == true)
	{
		m_nextWorldTime += deltaTime;
	}
	if (m_nextWorldTime >= 1.5f)
	{
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::ENDCUTSCENE);
	}

	// Fade in out
	if (m_tFadeInOut != nullptr)
	{
		m_tFadeInOut->GetUIInstance().lock()->UpdateFadeInOut(deltaTime);
	}

	if (m_isTrigger)
		StopMove();
}

void EndingFreeWorld::UpdateAnimation()
{
	const float MOVEMENT_THRESHOLD = 5.f;

	PxVec3 velocity = m_playerRigid->GetRigidDynamic()->getLinearVelocity();
	float speed = velocity.magnitude();

	bool currentlyMoving = speed > MOVEMENT_THRESHOLD;

	if (currentlyMoving != m_isMoving)
	{
		m_isMoving = currentlyMoving;

		if (m_isMoving)
		{
			if (WorldManager::GetInstance()->IsHostServer())
			{
				m_playerMesh->ChangeAnimation("../Resources/FBX/bear_move.fbx");
			}
			else
			{
				m_playerMesh->ChangeAnimation("../Resources/FBX/rabbit_move_.fbx");
			}
		}
		else
		{
			if (WorldManager::GetInstance()->IsHostServer())
			{
				m_playerMesh->ChangeAnimation("../Resources/FBX/bear_Idle.fbx");
			}
			else
			{
				m_playerMesh->ChangeAnimation("../Resources/FBX/rabbit_idle_.fbx");
			}
		}
	}
}


void EndingFreeWorld::OnEnter()
{
	InputManager::GetInstance()->GetMouse()->SetMode(Mouse::MODE_RELATIVE);
	World::OnEnter();
	InputManager::GetInstance()->AddInputProcesser(this);
}

void EndingFreeWorld::OnExit()
{
	InputManager::GetInstance()->GetMouse()->SetMode(Mouse::MODE_ABSOLUTE);
	World::OnExit();
	InputManager::GetInstance()->RemoveInputProcesser(this);
}

void EndingFreeWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}

void EndingFreeWorld::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
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
