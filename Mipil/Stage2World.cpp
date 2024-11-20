#include "pch.h"
#include "Stage2World.h"

#include "PlayerObject.h"
#include "BlockGenerator.h"
#include "PartGenerator.h"
#include "GridManager.h"

#include "DivisionMap.h"
#include "GoalParticleObject.h"
#include "FloorBlockObject.h"

#include "../Engine/CameraComponent.h"
#include "../Engine/GameObject.h"
#include "../Engine/UIFunctorPopUp.h"
#include "../Engine/UIObject.h"
#include "../Engine/UIPanel.h"
#include "../Engine/UIText.h"
#include "../Engine/WorldManager.h"
#include "../Engine/EventManager.h"
#include "../Engine/SoundManager.h"
#include "../Engine/RigidStaticComponent.h"

Stage2World::Stage2World()
	: PuzzleWorld()
{
}

Stage2World::~Stage2World()
{
}

void Stage2World::Initialize()
{
	LOG_CONTENTS("Stage2 Init");

	InitAll();

	std::shared_ptr<BlockGenerator> blockGenerator = std::make_shared<BlockGenerator>(shared_from_this(), "../Resources/Map/stage_2.yaml");
	std::shared_ptr<PartGenerator> partGenerator = std::make_shared<PartGenerator>(shared_from_this(), "../Resources/Map/stage_2.yaml");
	blockGenerator->Generate();
	partGenerator->Generate();
	SetStageParts(m_numParts);

	std::shared_ptr<FloorBlockObject> floorBlock = CreateGameObject<FloorBlockObject>("FloorBlock", eObjectType::TEST).lock();
	floorBlock->SetFilePath("../Resources/FBX/basetile5x6.fbx");
	std::shared_ptr<FloorBlockObject> floorBlock1 = CreateGameObject<FloorBlockObject>("FloorBlock", eObjectType::TEST).lock();
	floorBlock1->SetFilePath("../Resources/FBX/basepan.fbx");

	SetPlayersPosition({ 990.f, 90.f, 270.f }, { 990.f, 90.f, 630.f });
	// 턴 세팅
	SetTurnNum(5);

	// 목표 지점
	GridPos goal1 = { 0, 2, 4 };
	GridPos goal2 = { 0, 0, 3 };
	SetGoalGridPosition(goal1, goal2);

	// 목표 지점 파티클
	std::shared_ptr<GoalParticleObject> GoalParticleObject1 = CreateGameObject<GoalParticleObject>("GoalParticleObject1", eObjectType::PARTICLE).lock();
	std::shared_ptr<GoalParticleObject> GoalParticleObject2 = CreateGameObject<GoalParticleObject>("GoalParticleObject2", eObjectType::PARTICLE).lock();

	SetDivisionMapPosition({ 540.f, 0.f, 360.f });

	// 맵 전체 박스 콜라이더
	auto hollowBox = CreateGameObject<GameObject>("hollowBox", eObjectType::HOLLOWBOX).lock();
	auto hollowBoxRigidBody = hollowBox->CreateComponent<RigidStaticComponent>("hollowBox_Rigidbody");
	auto hollowBoxSceneComponent = hollowBox->CreateComponent<SceneComponent>("hollowBox_SceneComponent");
	hollowBox->SetRootComponent(hollowBoxSceneComponent.lock());
	hollowBoxRigidBody.lock()->CreateHollowBox(1080.f, 720.f, 900.f, { 0.f, 0.f, 0.f });

	// 미니맵 UI(홀로그램 패널 자식)
	auto MiniMapUI = m_UIManager->CreateUI<UIPanel>(L"Hologram_MiniMapUI");
	MiniMapUI->SetTexturePath(L"../Resources/Textures/1.png");
	MiniMapUI->SetPosition({ 479.f, 113.f });
	MiniMapUI->SetSize({ 494.f, 413.f });
	MiniMapUI->SetVisible(true);

	// 미니맵에 있는 Player(미니맵 UI 패널 자식)
	auto MiniMapPlayerUI = m_UIManager->CreateUI<UIPanel>(L"MiniMap_PlayerUI");
	MiniMapPlayerUI->SetTexturePath(L"../Resources/Textures/Input_player token.png");
	MiniMapPlayerUI->SetSize({ 50.f, 50.f });
	MiniMapPlayerUI->SetVisible(true);

	// 미니맵에 있는 friend(미니맵 UI 패널 자식)
	auto MiniMapFriendUI = m_UIManager->CreateUI<UIPanel>(L"MiniMap_FriendUI");
	MiniMapFriendUI->SetTexturePath(L"../Resources/Textures/Input_Friends token.png");
	MiniMapFriendUI->SetSize({ 50.f, 50.f });
	MiniMapFriendUI->SetVisible(true);
	MiniMapUI->AddChildren(MiniMapPlayerUI, MiniMapFriendUI);

	// 미니맵에 있는 target(미니맵 UI 패널 자식)
	for (int i = 0; i < m_numParts; i++)
	{
		std::string str = "MiniMap_TargetUI";
		str += std::to_string(i);
		auto targetUI = m_UIManager->CreateUI<UIPanel>(std::wstring().assign(str.begin(), str.end()));
		targetUI->SetTexturePath(L"../Resources/Textures/Input_target token.png");
		targetUI->SetSize({ 50.f, 50.f });
		targetUI->SetVisible(true);
		MiniMapUI->AddChildren(targetUI);
	}

	PuzzleWorld::Initialize();

	floorBlock->SetPosition({ 540.f, -60.f, 450.f });		// 바닥 블록 위치 설정
	floorBlock->SetRotation({ 0.f, 90.f, 0.f });		// 바닥 블록 회전 설정
	GoalParticleObject1->SetGoalPosition(goal1.x, goal1.y, goal1.z);
	GoalParticleObject2->SetGoalPosition(goal2.x, goal2.y, goal2.z);

	// 홀로그램에 미니맵 추가해주기
	auto hologramPanelUI = m_UIManager->GetUIObject<UIPanel>(L"Hologram_PanelUI");
	hologramPanelUI.lock()->AddChildren(MiniMapUI);

	// 스테이지 클리어 패널 로켓 이미지(스테이지 2은 45_2)
	auto stageClear_Rocket = m_UIManager->GetUIObject<UIPanel>(L"StageClear_Rocket");
	stageClear_Rocket.lock()->GetUIInstance().lock()->LoadTextureResource(L"../Resources/Textures/45_2.png");

	// 스테이지 클리어 패널 퍼센트(스테이지 2은 45%)
	auto rocketProcessPercent = m_UIManager->GetUIObject<UIText>(L"StageClear_RocketProcessPercent");
	rocketProcessPercent.lock()->SetText(L"45%");

	/// GridManager
	m_gridManager = std::make_shared<GridManager>();
	m_gridManager->CreateMap(6, 4, 5, 0.f, 0.f, 0.f, 180.f);
	m_gridManager->SetMapState(GetGameObjects(eObjectType::LEVEL));
	m_gridManager->SetMapState(GetGameObjects(eObjectType::PART));
	m_gridManager->SetMapState(GetGameObjects(eObjectType::PLAYER));

	// 미니맵 포지션 초기화
	InitMiniMap();

	m_hostPlayer.lock()->SetGridManager(m_gridManager);
	m_guestPlayer.lock()->SetGridManager(m_gridManager);

	// stage1에서는 HostPlayer한테 턴주기
	m_hostPlayer.lock()->SetTurn(true);
}

void Stage2World::Update(float deltaTime)
{
	// 3초동안 스테이지 클리어 팝업 보여주기
	if (m_bShowStageClear)
	{
		static bool once = true;
		if (once)
		{
			once = false;
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Stage_Clear.mp3");
		}

		auto StageClearPopUpUI_A = m_UIManager->GetUIObject<UIFunctorPopUp>(L"PopUpUI_StageClear");
		StageClearPopUpUI_A.lock()->SetAllVisible(true);

		m_dimUI->GetUIInstance().lock()->SetUseDim(true);

		m_showStageClear += deltaTime;
		m_bGameRun = false;
		if (m_showStageClear >= SHOW_STAGE_CLEAR)
		{
			once = true;
			m_showStageClear = 0.0f;
			m_bShowStageClear = false;
			StageClearPopUpUI_A.lock()->SetAllVisible(false);
			// todo 채원 :changeworld
			EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::STAGE3);
		}
	}

	PuzzleWorld::Update(deltaTime);
}

void Stage2World::OnEnter()
{
	PuzzleWorld::OnEnter();
}

void Stage2World::OnExit()
{
	PuzzleWorld::OnExit();
}