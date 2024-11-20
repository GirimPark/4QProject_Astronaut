#include "pch.h"
#include "GameOverWorld.h"

#include "UIRestartClickFunctor.h"
#include "UIGotoMainClickFunctor.h"
#include "UIGotoMainOkClick.h"

#include "../Engine/CameraComponent.h"

#include "../Engine/UIPanel.h"
#include "../Engine/UIButton.h"
#include "../Engine/UIFunctorPopUp.h"
#include "../Engine/WorldManager.h"

#include "../Engine/Event.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"

GameOverWorld::GameOverWorld()
{
	WorldManager::GetInstance()->RegisterHandler(S2C_RESTART_GAME, std::bind(&GameOverWorld::Network_Restart, this, std::placeholders::_1));
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, this);
	EventManager::GetInstance()->RegisterListener(eEventType::GO_TO_PREV_WORLD, this);
}

GameOverWorld::~GameOverWorld()
{
	EventManager::GetInstance()->UnregisterListener(eEventType::CHANGE_WORLD, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::GO_TO_PREV_WORLD, this);
}

void GameOverWorld::Initialize()
{
	//m_Camera = CreateGameObject<GameObject>("Camera", eObjectType::CAMERA).lock();
	//auto cameraComponent = m_Camera->CreateComponent<CameraComponent>("Camera_Component");
	//cameraComponent.lock()->SetLocalPosition({ 50.f, 100.f, -500.f });
	//m_Camera->SetRootComponent(cameraComponent.lock());

	m_bLoading = false;

	SoundManager::GetInstance()->StopAllSound();
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Stage_Gameover.mp3");
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Stage_Gameover.mp3");


	// 컷씬 이미지 3
	auto gameOverTexture = m_UIManager->CreateUI<UIPanel>(L"gameOverTexture");
	gameOverTexture->SetTexturePath(L"../Resources/Textures/game_over.jpg");
	gameOverTexture->SetSize({ 1920.f, 1080.f });
	gameOverTexture->SetPosition({ 0.f, 0.f });
	gameOverTexture->SetScale({ 1.f, 1.f });

	// 다시 시작 버튼 눌렀을 때 로딩 스프라이트 애니메이션
	auto LoadingPanelUI = m_UIManager->CreateUI<UIPanel>(L"LoadingPanelUI");
	LoadingPanelUI->SetTexturePath(L"../Resources/Textures/Over_Anim_Waiting for call.png");
	LoadingPanelUI->SetSize({ 684.f, 128.f });
	LoadingPanelUI->SetPosition({ 0.f,  64.f });
	LoadingPanelUI->SetScale({ 1.f, 1.f });
	LoadingPanelUI->SetVisible(false);

	// 게임 재시작 
	auto OverButton_Restart_UI = m_UIManager->CreateUI<UIButton, UIRestartClickFunctor>(L"OverButton_Restart_UI");
	OverButton_Restart_UI->SetTexturePath(L"../Resources/Textures/Over_btn_Restart.png");
	OverButton_Restart_UI->SetPosition({ -642.f, -395.f });
	OverButton_Restart_UI->SetSize({ 260.f, 70.f });	// 레디 버튼 펑터랑 UI (행동입력 패널 자식)
	OverButton_Restart_UI->SetVisible(true);

	// 게임 메인 화면으로 
	auto OverButton_ToMainScreen_UI = m_UIManager->CreateUI<UIButton, UIGotoMainClickFunctor>(L"OverButton_ToMainScreen_UI");
	OverButton_ToMainScreen_UI->SetTexturePath(L"../Resources/Textures/Over_btn_To main screen.png");
	OverButton_ToMainScreen_UI->SetPosition({ 585.f, -395.f });
	OverButton_ToMainScreen_UI->SetSize({ 374.f, 70.f });
	OverButton_ToMainScreen_UI->SetVisible(true);

	// 접속 종료 팝업
	auto ConnectionFailedPopUpUI = m_UIManager->CreateUI<UIFunctorPopUp>(L"ConnectionFailedPopUpUI");
	ConnectionFailedPopUpUI->SetTexturePath(L"../Resources/Textures/Over_p_ Connection failed.png");
	ConnectionFailedPopUpUI->SetSize({ 949.f, 548.f });
	ConnectionFailedPopUpUI->SetPosition({ 0.f, 0.f });
	ConnectionFailedPopUpUI->SetScale({ 1.f, 1.f });
	ConnectionFailedPopUpUI->SetVisible(false);

	// 레디 팝업에서 yes 버튼(레디 팝업 자식)
	auto ForTheFirstOk_UI = m_UIManager->CreateUI<UIButton, UIGotoMainOkClick>(L"ForTheFirstOk_UI");
	ForTheFirstOk_UI->SetTexturePath(L"../Resources/Textures/Over_p_btn_for the first time.png");
	ForTheFirstOk_UI->SetSize({ 282.f, 91.f });
	ForTheFirstOk_UI->SetPosition({ 0.f, -134.f });
	ForTheFirstOk_UI->SetScale({ 1.f, 1.f });
	ForTheFirstOk_UI->SetVisible(false);

	ConnectionFailedPopUpUI->AddChildren(ForTheFirstOk_UI);

	__super::Initialize();

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

	ConnectionFailedPopUpUI->SetAllVisible(false);
}

void GameOverWorld::Update(float deltaTime)
{
	World::Update(deltaTime);
}

void GameOverWorld::OnEnter()
{
	World::OnEnter();
}

void GameOverWorld::OnExit()
{

	World::OnExit();
}

void GameOverWorld::Network_Restart(char* pMsg)
{
	PacketS2C_RestartGame* rg = reinterpret_cast<PacketS2C_RestartGame*>(pMsg);
	if (rg->restart == '0')
	{
		// 접속이 종료되어 처음으로 어쩌구 팝업 띄우기
		auto connectionFailedPopUpUI = m_UIManager->GetUIObject<UIFunctorPopUp>(L"ConnectionFailedPopUpUI");
		connectionFailedPopUpUI.lock()->SetAllVisible(true);
	}
	else if (rg->restart == '1')
	{
		// HandleEvent ChangeWorld to prevWorld
		EventManager::GetInstance()->SendEvent(eEventType::GO_TO_PREV_WORLD, nullptr);
	}
	delete[] pMsg;
}

void GameOverWorld::HandleEvent(Event* event)
{
	switch (event->eventID)
	{
	case eEventType::CHANGE_WORLD:
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
		break;
	case eEventType::GO_TO_PREV_WORLD:
		WorldManager::GetInstance()->GoToPrevWorld();
		break;
	}
}
