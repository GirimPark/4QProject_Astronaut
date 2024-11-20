#include "pch.h"
#include "MainWorld.h"
#include "UIGameStartClick.h"
#include "UICreditClick.h"
#include "UIMainWorldButtonHover.h"
#include "UINoClick.h"
#include "UIEndGameOkClick.h"

#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"
#include "../Engine/UIButton.h"
#include "../Engine/UIClickPopUpFunctor.h"
#include "../Engine/UIFunctorPopUp.h"
#include "../Engine/UIPanel.h"
#include "../Engine/WorldManager.h"
#include "../Engine/UITextInputBox.h"

MainWorld::MainWorld()
{
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, this);
}

MainWorld::~MainWorld()
{
	EventManager::GetInstance()->UnregisterListener(eEventType::CHANGE_WORLD, this);
}

void MainWorld::Initialize()
{
	LOG_CONTENTS("MainWorld Init");

	SoundManager::GetInstance()->StopAllSound();
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/BGM/BGM_Scene_Title.mp3", FMOD_LOOP_NORMAL);
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/BGM/BGM_Scene_Title.mp3");

	// 타이틀 씬
	auto TitleScene = m_UIManager->CreateUI<UIObject>(L"TitleScene");
	TitleScene->SetTexturePath(L"../Resources/Textures/Sprite_Sheet.jpg");
	TitleScene->SetSize({ 1920, 1080 });
	TitleScene->SetPosition({ 0.f, 0.f });
	TitleScene->SetScale({ 1.f, 1.f });
	TitleScene->SetVisible(true);

	// 게임 시작 버튼 UI와 Functor
	auto GameStartFunctor = std::make_shared<UIMainWorldButtonHover>();
	GameStartFunctor->SetHoverTexturePath(L"../Resources/Textures/Main_btn_Start_game.png");

	auto GameStartUI = m_UIManager->CreateUI<UIButton, UIGameStartClick>(L"GameStartUI");
	GameStartUI->SetTexturePath(L"../Resources/Textures/Main_btn_Start game_defalt.png");
	GameStartUI->SetSize({ 235.f, 77.f });
	GameStartUI->SetPosition({ 0.f, -202.f });
	GameStartUI->SetScale({ 1.f, 1.f });
	GameStartUI->SetMouseHoverFunctor(GameStartFunctor);

	auto MainLogoUI = m_UIManager->CreateUI<UIPanel>(L"MainLogoUI");
	MainLogoUI->SetTexturePath(L"../Resources/Textures/Main_logo.png");
	MainLogoUI->SetSize({ 1650.f, 450.f });
	MainLogoUI->SetPosition({ 0.f, 315.f });
	MainLogoUI->SetScale({ 1.f, 1.f });

	// 크레딧 버튼 UI와 Functor
	auto CreditFunctor = std::make_shared<UIMainWorldButtonHover>();
	CreditFunctor->SetHoverTexturePath(L"../Resources/Textures/Main_btn_credit.png");

	auto CreditUI = m_UIManager->CreateUI<UIButton, UICreditClick>(L"CreditUI");
	CreditUI->SetTexturePath(L"../Resources/Textures/Main_btn_credit_defalt.png");
	CreditUI->SetSize({ 193.f, 75.f });
	CreditUI->SetPosition({ 0.f, -305.f });
	CreditUI->SetScale({ 1.f, 1.f });
	CreditUI->SetMouseHoverFunctor(CreditFunctor);

	// 게임 종료 버튼 UI와 Functor
	auto EndGameFunctor = std::make_shared<UIMainWorldButtonHover>();
	EndGameFunctor->SetHoverTexturePath(L"../Resources/Textures/Main_btn_End_game.png");

	auto EndGameUI = m_UIManager->CreateUI<UIButton>(L"EndGameUI");
	EndGameUI->SetTexturePath(L"../Resources/Textures/Main_btn_End game_defalt.png");
	EndGameUI->SetSize({ 235.f, 80.f });
	EndGameUI->SetPosition({ 0.f, -411.f });
	EndGameUI->SetScale({ 1.f, 1.f });
	EndGameUI->SetMouseHoverFunctor(EndGameFunctor);

	// 게임 종료 팝업 UI와 Functor
	auto EndGamePopUpUI = m_UIManager->CreateUI<UIFunctorPopUp>(L"EndGamePopUpUI");
	EndGamePopUpUI->SetTexturePath(L"../Resources/Textures/UIEndGamePopUp.png");
	EndGamePopUpUI->SetSize({ 1000.f, 1000.f });
	EndGamePopUpUI->SetPosition({ 0.f, 0.f });
	EndGamePopUpUI->SetScale({ 1.f, 1.f });

	auto EndGameClick = std::make_shared <UIClickPopUpFunctor>();
	EndGameClick->SetTargetUI(EndGamePopUpUI);

	auto EndGameOkUI = m_UIManager->CreateUI<UIButton, UIEndGameOkClick>(L"EndGameOkUI");
	EndGameOkUI->SetTexturePath(L"../Resources/Textures/UIEndGameOk.png");
	EndGameOkUI->SetSize({ 200.f, 100.f });
	EndGameOkUI->SetPosition({ -300.f, 0.f });
	EndGameOkUI->SetScale({ 1.f, 1.f });

	auto EndGameNoUI = m_UIManager->CreateUI<UIButton, UINoClick>(L"EndGameNoUI");
	EndGameNoUI->SetTexturePath(L"../Resources/Textures/UIEndGameNo.png");
	EndGameNoUI->SetSize({ 200.f, 100.f });
	EndGameNoUI->SetPosition({ 300.f, 0.f });
	EndGameNoUI->SetScale({ 1.f, 1.f });
	EndGamePopUpUI->AddChildren(EndGameOkUI, EndGameNoUI);

	EndGameUI->SetMouseClickFunctor(EndGameClick);

	// 방코드 입력 텍스트 박스
	////TextTextBoxUI
	//auto roomCodeInputUI = m_UIManager->CreateUI<UITextInputBox>(L"RoomCodeInputUI");
	//roomCodeInputUI->SetTexturePath(L"../Resources/Textures/TestRoomCodeUI.png");

	//auto uiInputText = m_UIManager->CreateUI<UIText>(L"UIInputText");
	//roomCodeInputUI->AddChildren(uiInputText);
	//roomCodeInputUI->SetUITextObject(uiInputText);

	//uiInputText->SetPosition({ 0.f, -300.f });

	//roomCodeInputUI->SetTextBoxSize({660.f, 580.f});
	//roomCodeInputUI->SetTextBoxPosition({0.f, 0.f});
	//roomCodeInputUI->SetScale({ 1.f, 1.f });
	//roomCodeInputUI->SetTextFont(L"Arial Rounded MT Bold");
	//roomCodeInputUI->SetTextFontSize(50.f);

	__super::Initialize();

	// 타이틀 씬
	{
		// 정방향
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				CB_UIAnimationKeyframe keyframe;
				keyframe.m_Offset.x = 1920.f * j + 2;
				keyframe.m_Offset.y = 1081.f * i + 8;
				keyframe.m_TextureSize.x = 15360.f;
				keyframe.m_TextureSize.y = 8640.f;
				keyframe.m_Size.x = 1910.f;
				keyframe.m_Size.y = 1065.f;
				keyframe.m_AnimationTime = 0.07f;
				TitleScene->AddKeyFrame(keyframe);
			}
		}
		//// 정방향 잔여물
		//for (int i = 0; i < 2; i++)
		//{
		//	CB_UIAnimationKeyframe keyframe;
		//	keyframe.m_Offset.x = 1920.f * i + 2;
		//	keyframe.m_Offset.y = 1080.f * 7 + 8;
		//	keyframe.m_TextureSize.x = 15360.f;
		//	keyframe.m_TextureSize.y = 8640.f;
		//	keyframe.m_Size.x = 1910.f;
		//	keyframe.m_Size.y = 1065.f;
		//	keyframe.m_AnimationTime = 0.03f;
		//	TitleScene->AddKeyFrame(keyframe);
		//}
		//// 역방향 잔여물
		//for (int i = 0; i < 2; i++)
		//{
		//	CB_UIAnimationKeyframe keyframe;
		//	keyframe.m_Offset.x = 1920.f * (1 - i) + 2;
		//	keyframe.m_Offset.y = 1080.f * 7 + 8;
		//	keyframe.m_TextureSize.x = 15360.f;
		//	keyframe.m_TextureSize.y = 8640.f;
		//	keyframe.m_Size.x = 1910.f;
		//
		//	keyframe.m_Size.y = 1065.f;
		//	keyframe.m_AnimationTime = 0.03f;
		//	TitleScene->AddKeyFrame(keyframe);
		//}
		// 역방향
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				CB_UIAnimationKeyframe keyframe;
				keyframe.m_Offset.x = 1920.f * (7 - j) + 2;
				keyframe.m_Offset.y = 1081.f * (6 - i) + 8;
				keyframe.m_TextureSize.x = 15360.f;
				keyframe.m_TextureSize.y = 8640.f;
				keyframe.m_Size.x = 1910.f;
				keyframe.m_Size.y = 1065.f;
				keyframe.m_AnimationTime = 0.07f;
				TitleScene->AddKeyFrame(keyframe);
			}
		}

		TitleScene->GetUIInstance().lock()->SetIsLoop(true);
	}
}

void MainWorld::Update(float deltaTime)
{
	World::Update(deltaTime);
}

void MainWorld::OnEnter()
{
	InputManager::GetInstance()->AddInputProcesser(this);

	World::OnEnter();
}

void MainWorld::OnExit()
{
	World::OnExit();

	InputManager::GetInstance()->RemoveInputProcesser(this);
}

void MainWorld::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}

void MainWorld::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
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
