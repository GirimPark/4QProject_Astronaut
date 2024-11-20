#include "pch.h"
#include "StartFreeWorld.h"

#include "../Engine/SoundManager.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"
#include "../Engine/CameraComponent.h"
#include "../Engine/RigidStaticComponent.h"
#include "../Engine/RigidDynamicComponent.h"
#include "../Engine/SceneComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/SkeletalMeshComponent.h"

#include "PlayerFreeCamera.h"
#include "PlayerFreeObject.h"
#include "ReadyCollisionHandler.h"
#include "MapObject.h"
#include "CubeMapObject.h"
#include "ShockParticleObjcect.h"
#include "StaticFbxObject.h"
#include "FireParticleObject.h"

#include <cmath>
#include <iomanip>   // std::setprecision
#include <sstream>   // std::ostringstream
#include <locale>
#include "SkeletalFbxObject.h"

void StartFreeWorld::Initialize()
{
	LOG_CONTENTS("StartFreeWorld Init");

	WorldManager::GetInstance()->RegisterHandler(S2C_IS_ALL_GOAL, std::bind(&StartFreeWorld::GotoStage1World, this, std::placeholders::_1));

	// Character //
	m_player = CreateGameObject<PlayerFreeObject>("FBX_TEST", eObjectType::PLAYER).lock();
	m_player->SetDeferredPosition({ -1487.f, 100.f, 6232.f });
	m_player->SetDeferredRotation({ 0.f, 28.4f, 0.f });

	auto fire = CreateGameObject<FireParticleObject>("fire", eObjectType::PARTICLE).lock();
	fire->SetParentObject(m_player);
	m_player->SetFireParticle(fire);

	// Camera //
	m_mainCamera = CreateGameObject<PlayerFreeCamera>("mainCamera", eObjectType::CAMERA).lock();
	m_mainCamera->SetSubject(m_player);
	m_player->SetCamera(m_mainCamera.get());


	auto pointEmpty = CreateGameObject<StaticFbxObject>("withStageMap", eObjectType::MAP).lock();
	pointEmpty->SetFilePath("../Resources/FBX/freemap_withStage.fbx");


	// Map //
	auto withStageMap = CreateGameObject<StaticFbxObject>("withStageMap", eObjectType::MAP).lock();
	withStageMap->SetFilePath("../Resources/FBX/freemap_withStage.fbx");

	auto collisionMap = CreateGameObject<MapObject>("collisionMap", eObjectType::MAP).lock();
//	collisionMap->SetFilePath("../Resources/FBX/freeMap_collsion_update.fbx");
	collisionMap->SetFilePath("../Resources/FBX/freeMap_collsion_obj.fbx");

	auto decoMap = CreateGameObject<StaticFbxObject>("decoMap", eObjectType::MAP).lock();
	decoMap->SetFilePath("../Resources/FBX/freeMap_decor_obj.fbx");

	auto readyTrigger = CreateGameObject<GameObject>("ReadyTrigger", eObjectType::READYTRIGGER).lock();
	auto triggerRigidBody = readyTrigger->CreateComponent<RigidStaticComponent>("Trigger_RigidBody").lock();
	auto triggerSceneComponent = readyTrigger->CreateComponent<SceneComponent>("Trigger_SceneComponent").lock();
	triggerSceneComponent->SetLocalPosition({ -175.f, 100.f, -4210.f });
	readyTrigger->SetRootComponent(triggerSceneComponent);
	triggerRigidBody->CreateStaticRigidBody(RigidBodyComponent::Geometry::Box, { 2000, 10, 10 }, { 0.5f, 0.5f, 0.6f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, true);
	triggerRigidBody->AddCollisionHandler<ReadyCollisionHandler>();

	// CubeMap
	CreateGameObject<CubeMapObject>("CubeMap", eObjectType::TEST);

	m_DistanceText = m_UIManager->CreateUI<UIText>(L"m_DistanceText");
	m_DistanceText->SetPosition({ 0.f, 450.f });
	m_DistanceText->SetFont(L"Akira Expanded");
	m_DistanceText->SetFontSize(30.f);
	m_DistanceText->SetColor(D2D1::ColorF::White);
	m_DistanceText->SetText(std::to_wstring(m_remainDistance));

	// Sound
	SoundManager::GetInstance()->StopAllSound();
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/BGM/BGM_Freemove.mp3", FMOD_LOOP_NORMAL);
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/BGM/BGM_Freemove.mp3", 0.5f);
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/BGM/BGM_Freemove.mp3");
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Freemove_Chatting.mp3");
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/Effect/Freemove_Chatting.mp3", 1.0f);
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Freemove_ChattingSend.mp3");
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/Effect/Freemove_ChattingSend.mp3", 1.0f);
	//SoundManager::GetInstance()->LoadSound("../Resources/Sound/bgm.mp3", FMOD_LOOP_NORMAL);
	//SoundManager::GetInstance()->SetVolume("../Resources/Sound/bgm.mp3", 0.1f);
	//SoundManager::GetInstance()->PlaySound("../Resources/Sound/bgm.mp3");


	// Chat Background
	{
		auto chat = m_UIManager->CreateUI<UIPanel>(L"chat");
		chat->SetTexturePath(L"../Resources/Textures/FreeMove_bg.png");
		chat->SetSize({ 1920.f, 1080.f });
		chat->SetPosition({ 0.f, 0.f });
		chat->SetScale({ 1.f, 1.f });

		float m_distanceTriggerArray[] = {
			5, 10, 15, 20, 35,
			40, 45, 50, 55, 60,
			65 ,70, 75, 80, 85, 90
		};


		if (WorldManager::GetInstance()->IsHostServer())
		{
			SetBearDialogue(m_distanceTriggerArray);		// if host
		}
		else
		{
			SetRabbitDialogue(m_distanceTriggerArray);		// if guest
		}
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
		m_destPosition = { -175.f, 100.f, -4210.f };
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

void StartFreeWorld::TriggerDim(bool dim)
{
	m_dimUI->GetUIInstance().lock()->SetUseDim(dim);

	auto LoadingPanelUI = m_UIManager->GetUIObject<UIPanel>(L"LoadingPanelUI");
	LoadingPanelUI.lock()->SetVisible(true);
}

void StartFreeWorld::StopMove()
{
	m_playerRigid->GetRigidDynamic()->clearForce();
	m_playerRigid->GetRigidDynamic()->clearTorque();
	m_playerRigid->GetRigidDynamic()->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));
	m_playerRigid->GetRigidDynamic()->setAngularVelocity(PxVec3(0.f, 0.f, 0.f));
	m_playerRigid->GetRigidDynamic()->putToSleep();

	m_remainDistance = 0.f;
}

void StartFreeWorld::DeletePlayerController()
{
	auto controllerCp = m_player->GetComponentByTypeName<ControllerComponent>().lock();
	if (controllerCp)
	{
		controllerCp->ClearControllers();
	}

	m_isTrigger = true;
}

void StartFreeWorld::Update(float deltaTime)
{
	World::Update(deltaTime);

	//Change Animation
	StartFreeWorld::UpdateAnimation();

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
				if (m_dialogueQueue.size() == 9 || m_dialogueQueue.size() == 7 || m_dialogueQueue.size() == 5 || m_dialogueQueue.size() == 3)
					SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Freemove_ChattingSend.mp3");
				else
					SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Freemove_Chatting.mp3");

				m_dialogueQueue.front().first->SetVisible(true);
				m_dialogueQueue.pop();
			}
		}
	}

	// Fade in out
	if (m_tFadeInOut != nullptr)
	{
		m_tFadeInOut->GetUIInstance().lock()->UpdateFadeInOut(deltaTime);
	}

	if (m_isTrigger)
		StopMove();
}

float StartFreeWorld::CalcDistance(Math::Vector3 startPos, Math::Vector3 destPos)
{
	// 높이는 거리에 상관없으므로 X,Z 축만으로 계산을 한다.
	float x = destPos.x - startPos.x;
	float z = destPos.z - startPos.z;
	float powXZ = pow(x, 2) + pow(z, 2);

	return std::sqrt(powXZ) * 0.01f;
}

void StartFreeWorld::SetRabbitDialogue(float trigger[])
{
	float textPad = 31.0f;

	// 1p Chat
	auto chatA = m_UIManager->CreateUI<UIPanel>(L"chatA");
	chatA->SetTexturePath(L"../Resources/ChatText/1p_a.png");
	chatA->SetSize({ 375.0f , 20.0f });
	chatA->SetPosition({ 720.f, 465.f });
	chatA->SetScale({ 1.f, 1.f });
	chatA->SetVisible(false);
	m_dialogueQueue.push({ chatA , trigger[0] });

	auto chatA2 = m_UIManager->CreateUI<UIPanel>(L"chatA2");
	chatA2->SetTexturePath(L"../Resources/ChatText/1p_b.png");
	chatA2->SetSize({ 375.0f , 20.0f });
	chatA2->SetPosition({ 720.f, 465.f - textPad });
	chatA2->SetScale({ 1.f, 1.f });
	chatA2->SetVisible(false);
	m_dialogueQueue.push({ chatA2, trigger[1] });

	auto chatC = m_UIManager->CreateUI<UIPanel>(L"chatC"); // 이미지 기준점 변경 필요
	chatC->SetTexturePath(L"../Resources/ChatText/1p_c.png");
	chatC->SetSize({ 375.0f , 20.0f });
	chatC->SetPosition({ 720.f, 465.f - textPad * 2 });
	chatC->SetScale({ 1.f, 1.f });
	chatC->SetVisible(false);
	m_dialogueQueue.push({ chatC , trigger[2] });

	auto chatD = m_UIManager->CreateUI<UIPanel>(L"chatD");
	chatD->SetTexturePath(L"../Resources/ChatText/1p_d.png");
	chatD->SetSize({ 375.0f , 20.0f });
	chatD->SetPosition({ 720.f, 465.f - textPad * 3 });
	chatD->SetScale({ 1.f, 1.f });
	chatD->SetVisible(false);
	m_dialogueQueue.push({ chatD, trigger[3] });

	auto chatE = m_UIManager->CreateUI<UIPanel>(L"chatE");
	chatE->SetTexturePath(L"../Resources/ChatText/1p_e.png");
	chatE->SetSize({ 375.0f , 20.0f });
	chatE->SetPosition({ 720.f, 465.f - textPad * 4 });
	chatE->SetScale({ 1.f, 1.f });
	chatE->SetVisible(false);
	m_dialogueQueue.push({ chatE, trigger[4] });

	auto chatF = m_UIManager->CreateUI<UIPanel>(L"chatF");
	chatF->SetTexturePath(L"../Resources/ChatText/1p_f.png");
	chatF->SetSize({ 375.0f , 20.0f });
	chatF->SetPosition({ 720.f, 465.f - textPad * 5 });
	chatF->SetScale({ 1.f, 1.f });
	chatF->SetVisible(false);
	m_dialogueQueue.push({ chatF, trigger[5] });

	auto chatG = m_UIManager->CreateUI<UIPanel>(L"chatG");
	chatG->SetTexturePath(L"../Resources/ChatText/1p_g.png");
	chatG->SetSize({ 375.0f , 20.0f });
	chatG->SetPosition({ 720.f, 465.f - textPad * 6 });
	chatG->SetScale({ 1.f, 1.f });
	chatG->SetVisible(false);
	m_dialogueQueue.push({ chatG, trigger[6] });

	auto chatH = m_UIManager->CreateUI<UIPanel>(L"chatH");
	chatH->SetTexturePath(L"../Resources/ChatText/1p_h.png");
	chatH->SetSize({ 375.0f , 20.0f });
	chatH->SetPosition({ 720.f, 465.f - textPad * 7 });
	chatH->SetScale({ 1.f, 1.f });
	chatH->SetVisible(false);
	m_dialogueQueue.push({ chatH, trigger[7] });

	auto chatI = m_UIManager->CreateUI<UIPanel>(L"chatI");
	chatI->SetTexturePath(L"../Resources/ChatText/1p_i.png");
	chatI->SetSize({ 375.0f , 20.0f });
	chatI->SetPosition({ 720.f, 465.f - textPad * 8 });
	chatI->SetScale({ 1.f, 1.f });
	chatI->SetVisible(false);
	m_dialogueQueue.push({ chatI, trigger[8] });

	auto chatI2 = m_UIManager->CreateUI<UIPanel>(L"chatI2");
	chatI2->SetTexturePath(L"../Resources/ChatText/1p_j.png");
	chatI2->SetSize({ 375.0f , 20.0f });
	chatI2->SetPosition({ 720.f, 465.f - textPad * 9 });
	chatI2->SetScale({ 1.f, 1.f });
	chatI2->SetVisible(false);
	m_dialogueQueue.push({ chatI2, trigger[9] });

	auto chatK = m_UIManager->CreateUI<UIPanel>(L"chatK");
	chatK->SetTexturePath(L"../Resources/ChatText/1p_k.png");
	chatK->SetSize({ 375.0f , 37.0f });
	chatK->SetPosition({ 720.f, 465.f - 7.5f - textPad * 10 });
	chatK->SetScale({ 1.f, 1.f });
	chatK->SetVisible(false);
	m_dialogueQueue.push({ chatK , trigger[10] });

	auto chatL = m_UIManager->CreateUI<UIPanel>(L"chatL");
	chatL->SetTexturePath(L"../Resources/ChatText/1p_l.png");
	chatL->SetSize({ 375.0f , 20.0f });
	chatL->SetPosition({ 720.f, 465.f - 14.0f - textPad * 11 });
	chatL->SetScale({ 1.f, 1.f });
	chatL->SetVisible(false);
	m_dialogueQueue.push({ chatL , trigger[11] });

	auto chatM = m_UIManager->CreateUI<UIPanel>(L"chatM");
	chatM->SetTexturePath(L"../Resources/ChatText/1p_m.png");
	chatM->SetSize({ 375.0f , 20.0f });
	chatM->SetPosition({ 720.f, 465.f - 12.0f - textPad * 12 });
	chatM->SetScale({ 1.f, 1.f });
	chatM->SetVisible(false);
	m_dialogueQueue.push({ chatM, trigger[12] });

	auto chatN = m_UIManager->CreateUI<UIPanel>(L"chatN");
	chatN->SetTexturePath(L"../Resources/ChatText/1p_n.png");
	chatN->SetSize({ 375.0f , 20.0f });
	chatN->SetPosition({ 720.f, 465.f - 9.5f - textPad * 13 });
	chatN->SetScale({ 1.f, 1.f });
	chatN->SetVisible(false);
	m_dialogueQueue.push({ chatN, trigger[13] });

	auto chatO = m_UIManager->CreateUI<UIPanel>(L"chatO");
	chatO->SetTexturePath(L"../Resources/ChatText/1p_o.png");
	chatO->SetSize({ 375.0f , 20.0f });
	chatO->SetPosition({ 720.f, 465.f - 9.5f - textPad * 14 });
	chatO->SetScale({ 1.f, 1.f });
	chatO->SetVisible(false);
	m_dialogueQueue.push({ chatO, trigger[14] });

}

void StartFreeWorld::GotoStage1World(char* pMsg)
{
	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STAGE1);
	delete[] pMsg;
}

void StartFreeWorld::SetBearDialogue(float trigger[])
{
	float textPad = 31.0f;

	// Bear Chat
	auto chatA = m_UIManager->CreateUI<UIPanel>(L"chatA");
	chatA->SetTexturePath(L"../Resources/ChatText/2p_a.png");
	chatA->SetSize({ 375.0f , 20.0f });
	chatA->SetPosition({ 720.f, 465.f });
	chatA->SetScale({ 1.f, 1.f });
	chatA->SetVisible(false);
	m_dialogueQueue.push({ chatA , trigger[0] });

	auto chatA2 = m_UIManager->CreateUI<UIPanel>(L"chatA2");
	chatA2->SetTexturePath(L"../Resources/ChatText/2p_b.png");
	chatA2->SetSize({ 375.0f , 20.0f });
	chatA2->SetPosition({ 720.f, 465.f - textPad });
	chatA2->SetScale({ 1.f, 1.f });
	chatA2->SetVisible(false);
	m_dialogueQueue.push({ chatA2, trigger[1] });

	auto chatC = m_UIManager->CreateUI<UIPanel>(L"chatC"); // 이미지 기준점 변경 필요
	chatC->SetTexturePath(L"../Resources/ChatText/2p_c.png");
	chatC->SetSize({ 375.0f , 20.0f });
	chatC->SetPosition({ 720.f, 465.f - textPad * 2 });
	chatC->SetScale({ 1.f, 1.f });
	chatC->SetVisible(false);
	m_dialogueQueue.push({ chatC , trigger[2] });

	auto chatD = m_UIManager->CreateUI<UIPanel>(L"chatD");
	chatD->SetTexturePath(L"../Resources/ChatText/2p_d.png");
	chatD->SetSize({ 375.0f , 20.0f });
	chatD->SetPosition({ 720.f, 465.f - textPad * 3 });
	chatD->SetScale({ 1.f, 1.f });
	chatD->SetVisible(false);
	m_dialogueQueue.push({ chatD, trigger[3] });

	auto chatE = m_UIManager->CreateUI<UIPanel>(L"chatE");
	chatE->SetTexturePath(L"../Resources/ChatText/2p_e.png");
	chatE->SetSize({ 375.0f , 20.0f });
	chatE->SetPosition({ 720.f, 465.f - textPad * 4 });
	chatE->SetScale({ 1.f, 1.f });
	chatE->SetVisible(false);
	m_dialogueQueue.push({ chatE, trigger[4] });

	auto chatF = m_UIManager->CreateUI<UIPanel>(L"chatF");
	chatF->SetTexturePath(L"../Resources/ChatText/2p_f.png");
	chatF->SetSize({ 375.0f , 20.0f });
	chatF->SetPosition({ 720.f, 465.f - textPad * 5 });
	chatF->SetScale({ 1.f, 1.f });
	chatF->SetVisible(false);
	m_dialogueQueue.push({ chatF, trigger[5] });

	auto chatG = m_UIManager->CreateUI<UIPanel>(L"chatG");
	chatG->SetTexturePath(L"../Resources/ChatText/2p_g.png");
	chatG->SetSize({ 375.0f , 20.0f });
	chatG->SetPosition({ 720.f, 465.f - textPad * 6 });
	chatG->SetScale({ 1.f, 1.f });
	chatG->SetVisible(false);
	m_dialogueQueue.push({ chatG, trigger[6] });

	auto chatH = m_UIManager->CreateUI<UIPanel>(L"chatH");
	chatH->SetTexturePath(L"../Resources/ChatText/2p_h.png");
	chatH->SetSize({ 375.0f , 20.0f });
	chatH->SetPosition({ 720.f, 465.f - textPad * 7 });
	chatH->SetScale({ 1.f, 1.f });
	chatH->SetVisible(false);
	m_dialogueQueue.push({ chatH, trigger[7] });

	auto chatI = m_UIManager->CreateUI<UIPanel>(L"chatI");
	chatI->SetTexturePath(L"../Resources/ChatText/2p_i.png");
	chatI->SetSize({ 375.0f , 20.0f });
	chatI->SetPosition({ 720.f, 465.f - textPad * 8 });
	chatI->SetScale({ 1.f, 1.f });
	chatI->SetVisible(false);
	m_dialogueQueue.push({ chatI, trigger[8] });

	auto chatI2 = m_UIManager->CreateUI<UIPanel>(L"chatI2");
	chatI2->SetTexturePath(L"../Resources/ChatText/2p_j.png");
	chatI2->SetSize({ 375.0f , 20.0f });
	chatI2->SetPosition({ 720.f, 465.f - textPad * 9 });
	chatI2->SetScale({ 1.f, 1.f });
	chatI2->SetVisible(false);
	m_dialogueQueue.push({ chatI2, trigger[9] });

	auto chatK = m_UIManager->CreateUI<UIPanel>(L"chatK");
	chatK->SetTexturePath(L"../Resources/ChatText/2p_k.png");
	chatK->SetSize({ 375.0f , 37.0f });
	chatK->SetPosition({ 720.f, 465.f - 7.5f - textPad * 10 });
	chatK->SetScale({ 1.f, 1.f });
	chatK->SetVisible(false);
	m_dialogueQueue.push({ chatK , trigger[10] });

	auto chatL = m_UIManager->CreateUI<UIPanel>(L"chatL");
	chatL->SetTexturePath(L"../Resources/ChatText/2p_l.png");
	chatL->SetSize({ 375.0f , 20.0f });
	chatL->SetPosition({ 720.f, 465.f - 14.0f - textPad * 11 });
	chatL->SetScale({ 1.f, 1.f });
	chatL->SetVisible(false);
	m_dialogueQueue.push({ chatL , trigger[11] });

	auto chatM = m_UIManager->CreateUI<UIPanel>(L"chatM");
	chatM->SetTexturePath(L"../Resources/ChatText/2p_m.png");
	chatM->SetSize({ 375.0f , 20.0f });
	chatM->SetPosition({ 720.f, 465.f - 12.0f - textPad * 12 });
	chatM->SetScale({ 1.f, 1.f });
	chatM->SetVisible(false);
	m_dialogueQueue.push({ chatM, trigger[12] });

	auto chatN = m_UIManager->CreateUI<UIPanel>(L"chatN");
	chatN->SetTexturePath(L"../Resources/ChatText/2p_n.png");
	chatN->SetSize({ 375.0f , 20.0f });
	chatN->SetPosition({ 720.f, 465.f - 9.5f - textPad * 13 });
	chatN->SetScale({ 1.f, 1.f });
	chatN->SetVisible(false);
	m_dialogueQueue.push({ chatN, trigger[13] });

	auto chatO = m_UIManager->CreateUI<UIPanel>(L"chatO");
	chatO->SetTexturePath(L"../Resources/ChatText/2p_o.png");
	chatO->SetSize({ 375.0f , 20.0f });
	chatO->SetPosition({ 720.f, 465.f - 9.5f - textPad * 14 });
	chatO->SetScale({ 1.f, 1.f });
	chatO->SetVisible(false);
	m_dialogueQueue.push({ chatO, trigger[14] });
}

void StartFreeWorld::UpdateAnimation()
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


void StartFreeWorld::OnEnter()
{
	InputManager::GetInstance()->GetMouse()->SetMode(Mouse::MODE_RELATIVE);
	World::OnEnter();
	InputManager::GetInstance()->AddInputProcesser(this);
}

void StartFreeWorld::OnExit()
{
	SoundManager::GetInstance()->TogglePlay("../Resources/Sound/BGM/BGM_Freemove.mp3");

	InputManager::GetInstance()->GetMouse()->SetMode(Mouse::MODE_ABSOLUTE);
	World::OnExit();
	InputManager::GetInstance()->RemoveInputProcesser(this);
}

void StartFreeWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}

void StartFreeWorld::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
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