#include "pch.h"
#include "StartingCutScene.h"

#include "../Engine/SoundManager.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"
#include "../Engine/UIPanel.h"

void StartingCutScene::Initialize()
{
	LOG_CONTENTS("StartingCutScene Init");

	// ToDo :: UIManager에 unordered_map 이라서 이미지 이름이 순서에 영향을 주는거 주의. //

	// 컷씬 이미지 4
	auto ImageD = m_UIManager->CreateUI<UIPanel>(L"ImageD");
	ImageD->SetTexturePath(L"../Resources/Textures/opning4_2.1.4.png");
	ImageD->SetSize({ 1920.f, 1080.f });
	ImageD->SetPosition({ 0.f, 0.f });
	ImageD->SetScale({ 1.f, 1.f });
	m_imageStack.push(ImageD);

	// 컷씬 이미지 3
	auto ImageC = m_UIManager->CreateUI<UIPanel>(L"ImageC");
	ImageC->SetTexturePath(L"../Resources/Textures/opning3_2.1.3.png");
	ImageC->SetSize({ 1920.f, 1080.f });
	ImageC->SetPosition({ 0.f, 0.f });
	ImageC->SetScale({ 1.f, 1.f });
	m_imageStack.push(ImageC);

	// 컷씬 이미지 2
	auto ImageB = m_UIManager->CreateUI<UIPanel>(L"ImageB");
	ImageB->SetTexturePath(L"../Resources/Textures/opning2_2.1.2.png");
	ImageB->SetSize({ 1920.f, 1080.f });
	ImageB->SetPosition({ 0.f, 0.f });
	ImageB->SetScale({ 1.f, 1.f });
	m_imageStack.push(ImageB);

	// 컷씬 이미지 1
	auto ImageA = m_UIManager->CreateUI<UIPanel>(L"ImageA");
	ImageA->SetTexturePath(L"../Resources/Textures/opning1_2.1.1.png");
	ImageA->SetSize({ 1920.f, 1080.f });
	ImageA->SetPosition({ 0.f, 0.f });
	ImageA->SetScale({ 1.f, 1.f });
	m_imageStack.push(ImageA);

	// Sound
	SoundManager::GetInstance()->StopAllSound();
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/BGM/BGM_Scene_OP.mp3", FMOD_LOOP_NORMAL);
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/BGM/BGM_Scene_OP.mp3", 0.5f);
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/OP_Rocket_Damaged.mp3");
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/OP_Rocket_Error.mp3");
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/OP_Rocket_Fire.mp3");
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/Effect/OP_Rocket_Damaged.mp3", 1.1f);
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/Effect/OP_Rocket_Error.mp3", 1.1f);
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/Effect/OP_Rocket_Fire.mp3", 1.1f);
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/bgm.mp3", FMOD_LOOP_NORMAL);
	SoundManager::GetInstance()->SetVolume("../Resources/Sound/bgm.mp3", 0.1f);
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/BGM/BGM_Scene_OP.mp3");

	// FadeInOut
	auto FadeInTestUI = m_UIManager->CreateUI<UIPanel>(L"Image");
	FadeInTestUI->SetSize({ 1920.f, 1080.f });
	FadeInTestUI->SetPosition({ 0.f, 0.f });
	FadeInTestUI->SetScale({ 1.0f, 1.0f });
	m_tFadeInOut = FadeInTestUI;

	m_delayTime = 1.5f;

	__super::Initialize();

	SoundManager::GetInstance()->PlaySound("../Resources/Sound/BGM/BGM_Scene_OP.mp3");

	// FadeInOut
	FadeInTestUI->GetUIInstance().lock()->SetIsFade();
	FadeInTestUI->GetUIInstance().lock()->SetFadeDurationTime(m_delayTime);
	m_tFadeInOut->GetUIInstance().lock()->FadeInStart();

}

void StartingCutScene::Update(float deltaTime)
{
	World::Update(deltaTime);

	if (m_tFadeInOut != nullptr)
	{
		m_tFadeInOut->GetUIInstance().lock()->UpdateFadeInOut(deltaTime);
	}


	if (m_bIsCanStartFade == true) // for 처음 시작할떄 페이드 인
	{
		if (m_skipDelayTime <= 0)
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/OP_Rocket_Fire.mp3");
		}

		m_skipDelayTime += deltaTime;
	}
	if (m_bIsCanStartFade == true && m_skipDelayTime > m_delayTime) // for 처음 시작할떄 페이드 인
	{
		m_bIsCanStartFade = false;
		m_skipDelayTime = 0;
	}

	if (m_skipDelayTime <= (m_delayTime * 2) && m_bStartCount == true) // timer start 
	{
		m_skipDelayTime += deltaTime;
	}
	if (m_skipDelayTime > m_delayTime && m_bIsCanFadeIn == true) // fade out End
	{
		m_bIsCanFadeIn = false;
		m_tFadeInOut->GetUIInstance().lock()->FadeInStart();
		m_imageStack.top()->SetVisible(false);
		m_imageStack.pop();

		if (m_imageStack.size() == 3)
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/OP_Rocket_Error.mp3");
		}
		else if (m_imageStack.size() == 2)
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/OP_Rocket_Damaged.mp3");
		}
	}
	if(m_skipDelayTime > m_delayTime * 2) // fade in End
	{
		m_bIsCanFadeOut = true;
		m_bIsCanFadeIn = true;
		m_bStartCount = false;
		m_skipDelayTime = 0.f;

		if (m_imageStack.size() == 3) // Crash Sound
		{
			// Crash Sound
			int a = 0;
		}

		if (m_imageStack.size() == 0)
		{
			// Next Stage
			EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STARTFREEWORLD);
		}
	}
}

void StartingCutScene::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
	const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		if (m_imageStack.size() > 0 && m_bIsCanFadeOut == true && m_bStartCount == false && m_bIsCanStartFade == false)
		{
			m_bStartCount = true;
			m_bIsCanFadeOut = false;
 			m_tFadeInOut->GetUIInstance().lock()->FadeOutStart();
		}
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

void StartingCutScene::OnEnter()
{
	InputManager::GetInstance()->AddInputProcesser(this);
	World::OnEnter();
}

void StartingCutScene::OnExit()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);
	World::OnExit();
}

void StartingCutScene::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}
