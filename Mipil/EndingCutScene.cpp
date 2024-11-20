#include "pch.h"
#include "EndingCutScene.h"

#include "../Engine/SoundManager.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"
#include "../Engine/UIPanel.h"

void EndingCutScene::Initialize()
{
	LOG_CONTENTS("EndingCutScene Init");

	SoundManager::GetInstance()->StopAllSound();
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/BGM/BGM_Scene_ED.mp3", FMOD_LOOP_NORMAL);
	SoundManager::GetInstance()->PlaySound("../Resources/Sound/BGM/BGM_Scene_ED.mp3");
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/ED_Zzazan.mp3");
	
	// ToDo :: UIManager에 unordered_map 이라서 이미지 이름이 순서에 영향을 주는거 주의. //

	// 컷씬 이미지 4
	auto ImageD = m_UIManager->CreateUI<UIPanel>(L"ImageD");
	ImageD->SetTexturePath(L"../Resources/Textures/ending4_3.1.4.png");
	ImageD->SetSize({ 1920.f, 1080.f });
	ImageD->SetPosition({ 0.f, 0.f });
	ImageD->SetScale({ 1.f, 1.f });
	m_imageStack.push(ImageD);

	// 컷씬 이미지 3
	auto ImageC = m_UIManager->CreateUI<UIPanel>(L"ImageC");
	ImageC->SetTexturePath(L"../Resources/Textures/ending3_3.1.3.png");
	ImageC->SetSize({ 1920.f, 1080.f });
	ImageC->SetPosition({ 0.f, 0.f });
	ImageC->SetScale({ 1.f, 1.f });
	m_imageStack.push(ImageC);

	// 컷씬 이미지 2
	auto ImageB = m_UIManager->CreateUI<UIPanel>(L"ImageB");
	ImageB->SetTexturePath(L"../Resources/Textures/ending2_3.1.2.png");
	ImageB->SetSize({ 1920.f, 1080.f });
	ImageB->SetPosition({ 0.f, 0.f });
	ImageB->SetScale({ 1.f, 1.f });
	m_imageStack.push(ImageB);

	// 컷씬 이미지 1
	auto ImageA = m_UIManager->CreateUI<UIPanel>(L"ImageA");
	ImageA->SetTexturePath(L"../Resources/Textures/ending1_3.1.1.png");
	ImageA->SetSize({ 1920.f, 1080.f });
	ImageA->SetPosition({ 0.f, 0.f });
	ImageA->SetScale({ 1.f, 1.f });
	m_imageStack.push(ImageA);

	// FadeInOut
	auto FadeInTestUI = m_UIManager->CreateUI<UIPanel>(L"Image");
	FadeInTestUI->SetSize({ 1920.f, 1080.f });
	FadeInTestUI->SetPosition({ 0.f, 0.f });
	FadeInTestUI->SetScale({ 1.0f, 1.0f });
	m_tFadeInOut = FadeInTestUI;

	m_delayTime = 1.5f;

	__super::Initialize();

	// FadeInOut
	FadeInTestUI->GetUIInstance().lock()->SetIsFade();
	FadeInTestUI->GetUIInstance().lock()->SetFadeDurationTime(m_delayTime);
	m_tFadeInOut->GetUIInstance().lock()->FadeInStart();
}

void EndingCutScene::Update(float deltaTime)
{
	World::Update(deltaTime);

	if (m_tFadeInOut != nullptr)
	{
		m_tFadeInOut->GetUIInstance().lock()->UpdateFadeInOut(deltaTime);
	}

	if (m_bIsCanStartFade == true) // for 처음 시작할때 페이드 인
	{
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
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/ED_Zzazan.mp3");
		}
	}
	if (m_skipDelayTime > m_delayTime * 2) // fade in End
	{
		m_bIsCanFadeOut = true;
		m_bIsCanFadeIn = true;
		m_bStartCount = false;
		m_skipDelayTime = 0.f;



		if (m_imageStack.size() == 0)
		{
			// Next Stage
			EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::CREDIT);
		}
	}
}

void EndingCutScene::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
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

void EndingCutScene::OnEnter()
{
	InputManager::GetInstance()->AddInputProcesser(this);
	World::OnEnter();
}

void EndingCutScene::OnExit()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);
	World::OnExit();
}

void EndingCutScene::HandleEvent(Event* event)
{
	if (event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}
