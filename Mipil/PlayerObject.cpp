#include "pch.h"
#include "PlayerObject.h"

#include "GridManager.h"
#include "PlayerFSM.h"
#include "PuzzleWorld.h"
#include "GridMovementComponent.h"
#include "OnGridCollisionHandler.h"
#include "FireParticleObject.h"
#include "PointLightComponent.h"

#include "../Engine/SkeletalMeshComponent.h"
#include "../Engine/FSMComponent.h"
#include "../Engine/EventManager.h"
#include "../Engine/UIPanel.h"
#include "../Engine/WorldManager.h"
#include "../Engine/RigidDynamicComponent.h"
#include "../Engine/UIMeshComponent.h"
#include "../Engine/MaterialManagerComponent.h"
#include "../Engine/SoundManager.h"
#include "../D3DRenderer/SkeletalMeshModel.h"

PlayerObject::PlayerObject()
{
	InputManager::GetInstance()->AddInputProcesser(this);

	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_ANIMATION, this);
	EventManager::GetInstance()->RegisterListener(eEventType::MOVE_ON_GRID, this);
	EventManager::GetInstance()->RegisterListener(eEventType::CRASH_ON_GRID, this);
	EventManager::GetInstance()->RegisterListener(eEventType::PLAY_FIRE_PARTICLE, this);

	m_actions.resize(4);
}

PlayerObject::~PlayerObject()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);

	EventManager::GetInstance()->UnregisterListener(eEventType::CHANGE_ANIMATION, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::MOVE_ON_GRID, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::CRASH_ON_GRID, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::PLAY_FIRE_PARTICLE, this);
}

void PlayerObject::Initialize()
{
	m_playerFSM = std::make_shared<PlayerFSM>();

	m_fsmComponent = CreateComponent<FSMComponent>("SkeletalFSMComponent").lock();
	m_playerFSM->SetOwnerObject(this);
	// playerFSM 생성자에 있는 AddAnimation만 빼서 호출
	m_fsmComponent->AttachFSM(m_playerFSM);

	// 사운드 로드
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Command_Click.mp3");
	SoundManager::GetInstance()->LoadSound("../Resources/Sound/Effect/Command_Delete.mp3");


	// mesh Component
	m_meshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent").lock();
	m_meshComponent->SetDefaultAnimation(m_playerFSM->GetAnimationName("State_Idle"), true);
	m_meshComponent->AddAnimation(m_playerFSM->GetAnimationName("State_TurnWait"), true);
	m_meshComponent->AddAnimation(m_playerFSM->GetAnimationName("State_TurnMove"), true);
	SetRootComponent(m_meshComponent);
	SetScale({ 0.5f, 0.5f, 0.5f });
	m_meshComponent->SetOffsetPosition({ 0.f, -67.f, 0.f });

	// 3D UI Mesh Component 생성
	m_uiMeshComponent = CreateComponent<UIMeshComponent>("UIMeshComponent").lock();
	m_uiMeshComponent->SetDefaultAnimation(m_playerFSM->GetAnimationName("State_Idle_X"), true);
	m_uiMeshComponent->AddAnimation(m_playerFSM->GetAnimationName("State_TurnWait_X"), true);
	m_uiMeshComponent->AddAnimation(m_playerFSM->GetAnimationName("State_TurnMove_X"), true);
	m_uiMeshComponent->SetOutLine(false);
	m_uiMeshComponent->SetUIMove(false);
	m_uiMeshComponent->SetPlayerNumber(m_bHostPlayer + 1);
	m_uiMeshComponent->SetVisible(false);

	//uiMatrix = Matrix::CreateRotationX(45.f) * Matrix::CreateRotationY(45.f) * Matrix::CreateRotationZ(45.f);
	//m_uiMeshComponent->GetSkeletalMeshModel()->SetWorldMatrix(&uiMatrix);

	// Mesh Material Manager Component 생성
	{
		m_materialManagerComponent1 = CreateComponent<MaterialManagerComponent>("PlayerMaterialManagerComponent").lock();
		m_materialManagerComponent2 = CreateComponent<MaterialManagerComponent>("PlayerMaterialManagerComponent").lock();
		m_materialManagerComponent3 = CreateComponent<MaterialManagerComponent>("PlayerMaterialManagerComponent").lock();

		// 곰 웃는 표정
		m_materialManagerComponent1->MaterialSetting("SmileBearBodyMaterial", L"../Resources/Textures/bear_smile_Base_color.png", MaterialMapFlags::BASECOLOR);
		m_materialManagerComponent1->MaterialSetting("SmileBearBodyMaterial", L"../Resources/Textures/bear_smile_Normal_OpenGL.png", MaterialMapFlags::NORMAL);
		m_materialManagerComponent1->MaterialSetting("SmileBearBodyMaterial", L"../Resources/Textures/bear_body&face_Roughness_11.png", MaterialMapFlags::ROUGHNESS);
		m_materialManagerComponent1->MaterialSetting("SmileBearGoggleMaterial", L"../Resources/Textures/bear_goggle_Base_color.png", MaterialMapFlags::BASECOLOR);
		m_materialManagerComponent1->MaterialSetting("SmileBearGoggleMaterial", L"../Resources/Textures/bear_goggle_Normal_OpenGL.png", MaterialMapFlags::NORMAL);
		m_materialManagerComponent1->MaterialSetting("SmileBearGoggleOpacityMaterial", L"../Resources/Textures/bear_goggle_glass_Base_color_.png", MaterialMapFlags::BASECOLOR);
		m_materialManagerComponent1->MaterialSetting("SmileBearGoggleOpacityMaterial", L"../Resources/Textures/bear_goggle_Opacity.png", MaterialMapFlags::OPACITY);
		// 곰 충격 받은 표정
		m_materialManagerComponent1->MaterialSetting("CrashBearBodyMaterial", L"../Resources/Textures/bear_dizzy_Base_color.png", MaterialMapFlags::BASECOLOR);
		m_materialManagerComponent1->MaterialSetting("CrashBearBodyMaterial", L"../Resources/Textures/bear_dizzy_Normal_OpenGL.png", MaterialMapFlags::NORMAL);
		m_materialManagerComponent1->MaterialSetting("CrashBearBodyMaterial", L"../Resources/Textures/bear_body&face_Roughness_11.png", MaterialMapFlags::ROUGHNESS);
		m_materialManagerComponent1->MaterialSetting("CrashBearGoggleMaterial", L"../Resources/Textures/bear_goggle_Base_color.png", MaterialMapFlags::BASECOLOR);
		m_materialManagerComponent1->MaterialSetting("CrashBearGoggleMaterial", L"../Resources/Textures/bear_goggle_Normal_OpenGL.png", MaterialMapFlags::NORMAL);
		m_materialManagerComponent1->MaterialSetting("CrashBearGoggleOpacityMaterial", L"../Resources/Textures/bear_goggle_glass_Base_color_.png", MaterialMapFlags::BASECOLOR);
		m_materialManagerComponent1->MaterialSetting("CrashBearGoggleOpacityMaterial", L"../Resources/Textures/bear_goggle_Opacity.png", MaterialMapFlags::OPACITY);

		// 토끼 웃는 표정
		m_materialManagerComponent1->MaterialSetting("SmileRabbitBodyMaterial", L"../Resources/Textures/rabbit_smile_Base_color.png", MaterialMapFlags::BASECOLOR);
		m_materialManagerComponent1->MaterialSetting("SmileRabbitBodyMaterial", L"../Resources/Textures/bear_dizzy_Normal_OpenGL.png", MaterialMapFlags::NORMAL);
		m_materialManagerComponent1->MaterialSetting("SmileRabbitBodyMaterial", L"../Resources/Textures/bear_body&face_Roughness_11.png", MaterialMapFlags::ROUGHNESS);
		m_materialManagerComponent1->MaterialSetting("SmileRabbitGoggleMaterial", L"../Resources/Textures/rabbit_goggle_Base_color.png", MaterialMapFlags::BASECOLOR);
		// 토끼 충격 받은 표정
		m_materialManagerComponent1->MaterialSetting("CrashRabbitBodyMaterial", L"../Resources/Textures/rabbit_dizzy_Base_color.png", MaterialMapFlags::BASECOLOR);
		m_materialManagerComponent1->MaterialSetting("CrashRabbitBodyMaterial", L"../Resources/Textures/rabbit_dizzy_Normal_OpenGL.png", MaterialMapFlags::NORMAL);
		m_materialManagerComponent1->MaterialSetting("CrashRabbitBodyMaterial", L"../Resources/Textures/bear_body&face_Roughness_11.png", MaterialMapFlags::ROUGHNESS);
		m_materialManagerComponent1->MaterialSetting("CrashRabbitGoggleMaterial", L"../Resources/Textures/rabbit_goggle_Base_color.png", MaterialMapFlags::BASECOLOR);
	}

	auto pointLight = CreateComponent<PointLightComponent>("Test_PointLight1").lock();
	pointLight->SetRange(100);
	pointLight->SetPointLightIntencity(30);
	pointLight->SetLightColor(1, 0, 0);

	// rigidbody 생성 전 반드시 위치를 필요로하기 때문에 플레이어오브젝트는 따로 초기 위치 설정을 한다.
	SetPosition(m_deferredPosition);

	m_gridMovementComponent = CreateComponent<GridMovementComponent>("Player_GridMovement_Component").lock();
	m_rigidDynamicComponent = CreateComponent<RigidDynamicComponent>("Capsule_Rigidbody").lock();
	m_rigidDynamicComponent->CreateDynamicRigidBody(RigidBodyComponent::Geometry::Capsule, { 55.f, 15.f }, { 0.5f, 0.5f, 0.0f }, 0.001f);
	m_rigidDynamicComponent->SetLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	m_rigidDynamicComponent->AddCollisionHandler<OnGridCollisionHandler>();

	m_bTurn = false;

	GameObject::Initialize();
}

void PlayerObject::Update(float deltaTime)
{
	if (m_bstayTimer)
	{
		m_stayTimer += deltaTime;

		if (m_stayTimer >= 1.0f)
		{
			MovementFinished();

			if (m_bTurn)
			{
				if (m_bChangeTurn)
				{
					ChangeFinishedActionUI();
					InCreaseIndex();

					m_bChangeTurn = false;
					std::shared_ptr<PuzzleWorld> pPuzzleWorld = std::dynamic_pointer_cast<PuzzleWorld>(GetOwnerWorld().lock());
					if (pPuzzleWorld->CheckRemainingAction())
					{
						/*Network_ChangeTurn();
						WorldManager::GetInstance()->PushSendQueue(
							WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_StartTurn), C2S_START_TURN, nullptr),
							sizeof(PacketC2S_StartTurn));*/
							/*PacketC2S_MovementEnd me;
							if (m_bHostPlayer)
								me.who = '0';
							else
								me.who = '1';
							WorldManager::GetInstance()->PushSendQueue(
								WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_MovementEnd), C2S_MOVEMENT_END, &me.who),
								sizeof(PacketC2S_MovementEnd));*/
						m_bWaitingChange = true;
					}
				}
			}
			m_stayTimer = 0.0f;
			m_bstayTimer = false;
		}
	}

	GameObject::Update(deltaTime);
}

// 서버에서 받은 4가지 행동을 각 플레이어의 m_action이라는 컨테이너에 넣어준다.
void PlayerObject::SetActions(char actions[])
{
	for (int i = 0; i < 4; i++)
	{
		m_actions[i] = CharToVec(actions[i]);
	}
}

void PlayerObject::SetLoadingActionUI()
{
	std::shared_ptr<PuzzleWorld> pPuzzleWorld = std::dynamic_pointer_cast<PuzzleWorld>(GetOwnerWorld().lock());
	auto loadingPanelUI = GetOwnerWorld().lock()->GetUIManager()->GetUIObject<UIPanel>(L"LoadingPanelUI");
	auto hologramPanelUI = GetOwnerWorld().lock()->GetUIManager()->GetUIObject<UIPanel>(L"Hologram_PanelUI");
	auto loadingActionPanelUI = GetOwnerWorld().lock()->GetUIManager()->GetUIObject<UIPanel>(L"InputAction_LoadingActionPanelUI");
	loadingActionPanelUI.lock()->SetAllVisible(true);
	for (short i = 0; i < 4; i++)
	{
		auto ui = pPuzzleWorld->GetUIManager()->GetUIObject<UIPanel>(pPuzzleWorld->ExtractUIName("LoadingPanel_ActionUI", std::to_string(i)));
		ui.lock()->GetUIInstance().lock()->LoadTextureResource(pPuzzleWorld->ConvertToActionUI(m_actions[i], false));
	}

	loadingPanelUI.lock()->SetAllVisible(true);
	hologramPanelUI.lock()->SetAllVisible(false);
}

void PlayerObject::SetExecuteBehavior(bool isExecuteBehavior)
{
	m_uiMeshComponent->SetVisible(isExecuteBehavior);
}

// 클라에서 받은 행동 4가지를 서버에 send 해준다.(각 플레이어의 m_action에 채워주기 위함.)
void PlayerObject::Network_SendAction()
{
	PacketC2S_BroadcastAction ba;

	if (m_bHostPlayer)
		ba.who = '0';
	else if (!m_bHostPlayer)
		ba.who = '1';

	for (int i = 0; i < 4; i++)
	{
		ba.action[i] = VecToChar(m_actions[i]);
	}

	WorldManager::GetInstance()->PushSendQueue(
		WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_BroadcastAction), C2S_BROADCAST_ACTION, &ba.who),
		sizeof(PacketC2S_BroadcastAction));
}

// 서버에 누구의 행동인지와 어떤 행동인지 send 해준다.
void PlayerObject::ProceedAction()
{
	// UI Mesh 외곽선 세팅
	m_uiMeshComponent->SetOutLine(true);

	// 그라운드 체크
	if (m_playerFSM->IsIdleState())
		m_gridManager->FallToGround(this);

	PuzzleWorld* world = dynamic_cast<PuzzleWorld*>(m_pOwnerWorld.lock().get());
	assert(world);
	UnComplexPlayer(world->GetOtherPlayer(this));

	// 합체 체크
	ComplexCheck();

	// Event : 행동을 처리하기 전에 턴이 시작되었음, 이번 행동, 아래 그리드 상태를 transition에 알린다.(...->Turn_State)
	EventManager::GetInstance()->SendImmediateEvent(eEventType::CHANGE_TURN, nullptr, Event_TurnInfo{ m_bHostPlayer, true });
	EventManager::GetInstance()->SendImmediateEvent(eEventType::GET_THIS_ACTION, nullptr, m_actions[m_actionIndex]);
	EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_GROUND, nullptr, Event_GroundInfo{ m_bHostPlayer, m_gridManager->IsGround(GetWorldPosition()) });
	// 행동 처리
	m_gridManager->MoveOnGrid(this, m_actions[m_actionIndex]);
}

void PlayerObject::Network_ChangeTurn()
{
	m_uiMeshComponent->SetOutLine(false);

	// 턴 체인지 해주기
	PacketC2S_SetTurn st;

	if (m_bHostPlayer)	// 호스트가 턴일 때 게스트 턴으로 SetTurn해주기
	{
		st.who = '1';
		WorldManager::GetInstance()->PushSendQueue(
			WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_SetTurn), C2S_SET_TURN, &st.who),
			sizeof(PacketC2S_SetTurn));
	}
	else
	{
		st.who = '0';
		WorldManager::GetInstance()->PushSendQueue(
			WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_SetTurn), C2S_SET_TURN, &st.who),
			sizeof(PacketC2S_SetTurn));
	}
}

// 행동 4가지가 다 끝났는지
bool PlayerObject::CheckActionEnd()
{
	// 끝내기
	if (m_actionIndex >= 4)
	{
		return true;
	}
	return false;
}

void PlayerObject::ResetIndex()
{
	m_actionIndex = 0;
	m_loadingIndex = 0;
}

// 제한시간이 지났을 때 모두 STAY로 바꿔주기
void PlayerObject::TimeOut()
{
	for (m_loadingIndex; m_loadingIndex < 4; m_loadingIndex++)
	{
		std::shared_ptr<PuzzleWorld> pPuzzleWorld = std::dynamic_pointer_cast<PuzzleWorld>(GetOwnerWorld().lock());
		auto ui = pPuzzleWorld->GetUIManager()->GetUIObject<UIPanel>(pPuzzleWorld->ExtractUIName("Hologram_UIAction", std::to_string(m_loadingIndex)));
		ui.lock()->GetUIInstance().lock()->LoadTextureResource(L"../Resources/Textures/Input_action_q(wait).png");
		ui.lock()->SetVisible(true);
		m_actions[m_loadingIndex] = grid::STAY;
	}
}

void PlayerObject::ChangeFinishedActionUI()
{
	std::shared_ptr<PuzzleWorld> pPuzzleWorld = std::dynamic_pointer_cast<PuzzleWorld>(GetOwnerWorld().lock());
	std::string str;
	if (m_bHostPlayer)
	{
		str = "Turn_HostAction";
	}
	else
	{
		str = "Turn_GuestAction";
	}
	str += std::to_string(m_actionIndex);
	auto ui = pPuzzleWorld->GetUIManager()->GetUIObject<UIPanel>(std::wstring().assign(str.begin(), str.end()));
	ui.lock()->SetVisible(true);
	ui.lock()->GetUIInstance().lock()->LoadTextureResource(pPuzzleWorld->ConvertToActionUI(m_actions[m_actionIndex], true));
}

void PlayerObject::SmaileMaterial(bool isComeback, int comebackTime)
{
	if (m_bHostPlayer)
	{
		m_materialManagerComponent1->ChangedMaterial(m_uiMeshComponent, "SmileBearBodyMaterial", 0, isComeback, comebackTime);
		//m_materialManagerComponent2->ChangedMaterial(m_uiMeshComponent, "SmileBearGoggleMaterial", 3, isComeback, comebackTime);
		//m_materialManagerComponent3->ChangedMaterial(m_uiMeshComponent, "SmileBearGoggleOpacityMaterial", 4, isComeback, comebackTime);
	}
	else
	{
		//m_materialManagerComponent1->ChangedMaterial(m_uiMeshComponent, "SmileRabbitGoggleMaterial", 1, isComeback, comebackTime);
		m_materialManagerComponent2->ChangedMaterial(m_uiMeshComponent, "SmileRabbitBodyMaterial", 0, isComeback, comebackTime);
	}
}

void PlayerObject::CrashMaterial(bool isComeback, int comebackTime)
{
	if (m_bHostPlayer)
	{
		m_materialManagerComponent1->ChangedMaterial(m_uiMeshComponent, "CrashBearBodyMaterial", 0, isComeback, comebackTime);
		//m_materialManagerComponent2->ChangedMaterial(m_uiMeshComponent, "CrashBearGoggleMaterial", 3, isComeback, comebackTime);
		//m_materialManagerComponent3->ChangedMaterial(m_uiMeshComponent, "CrashBearGoggleOpacityMaterial", 4, isComeback, comebackTime);
	}
	else
	{
		//m_materialManagerComponent1->ChangedMaterial(m_uiMeshComponent, "CrashRabbitGoggleMaterial", 1, isComeback, comebackTime);
		m_materialManagerComponent2->ChangedMaterial(m_uiMeshComponent, "CrashRabbitBodyMaterial", 0, isComeback, comebackTime);
	}
}

void PlayerObject::InitPlayer()
{
	m_actions.clear();
	m_actions.resize(4);
	m_bTurn = false;
	m_bReadyState = false;
	m_actionIndex = 0;
	m_loadingIndex = 0;
	m_stayTimer = 0.0f;
	m_bstayTimer = false;
	m_bChangeTurn = false;
	m_bWaitingChange = false;
	m_getParts = 0;
	auto movementComp = GetComponentByTypeName<GridMovementComponent>().lock();
	movementComp->InitGridMovement();
}

void PlayerObject::FallToGround()
{
	m_gridManager->FallToGround(this);
}

bool PlayerObject::IsIdleState()
{
	return m_playerFSM->IsIdleState();
}

void PlayerObject::MovementFinished()
{
	EventManager::GetInstance()->SendImmediateEvent(eEventType::CHANGE_TURN, nullptr, Event_TurnInfo{ m_bHostPlayer, false });
	EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_GROUND, nullptr, Event_GroundInfo{ m_bHostPlayer, m_gridManager->IsGround(GetWorldPosition()) });
	m_uiMeshComponent->SetUIMove(false);
}

char PlayerObject::VecToChar(Vector3& vec)
{
	if (vec == grid::X_UP)
		return '0';

	else if (vec == grid::X_DOWN)
		return '1';

	else if (vec == grid::Y_UP)
		return '2';

	else if (vec == grid::Y_DOWN)
		return '3';

	else if (vec == grid::Z_UP)
		return '4';

	else if (vec == grid::Z_DOWN)
		return '5';

	else if (vec == grid::STAY)
		return '6';
}

Vector3 PlayerObject::CharToVec(char& buf)
{
	if (buf == '0')
		return grid::X_UP;

	else if (buf == '1')
		return grid::X_DOWN;

	else if (buf == '2')
		return grid::Y_UP;

	else if (buf == '3')
		return grid::Y_DOWN;

	else if (buf == '4')
		return grid::Z_UP;

	else if (buf == '5')
		return grid::Z_DOWN;

	else if (buf == '6')
		return grid::STAY;
}

void PlayerObject::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	std::shared_ptr<PuzzleWorld> pObj = std::dynamic_pointer_cast<PuzzleWorld>(GetOwnerWorld().lock());

	// 입력 받을 때 예외처리
	// 예외처리 1 : 인게임 진행중일 때
	// 예외처리 2 : 게임 시작한다고 팝업 나올 때
	// 예외처리 3 : 플레이어가 레디상태 일 때
	// 예외처리 4 : 레디버튼 눌러서 팝업 떠있을 때는 조작 못하게 예외처리
	// 예외처리 5 : 이미 행동 4개가 입력 되어있을 때(UI인덱스 벗어나서 예외처리 해줌)
	// World에서 인풋 막으면 여기도 막히게 하기

	if (pObj->GetBanActionInput())
		return;

	if ((WorldManager::GetInstance()->IsHostServer() && m_bHostPlayer)
		|| (!WorldManager::GetInstance()->IsHostServer() && !m_bHostPlayer))
	{

		if (KeyTracker.IsKeyPressed(DirectX::Keyboard::W))
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Command_Click.mp3");

			if (m_loadingIndex >= 4)
				return;
			m_actions[m_loadingIndex++] = grid::Z_UP;
		}
		else if (KeyTracker.IsKeyPressed(DirectX::Keyboard::S))
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Command_Click.mp3");

			if (m_loadingIndex >= 4)
				return;
			m_actions[m_loadingIndex++] = grid::Z_DOWN;
		}
		else if (KeyTracker.IsKeyPressed(DirectX::Keyboard::D))
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Command_Click.mp3");

			if (m_loadingIndex >= 4)
				return;
			m_actions[m_loadingIndex++] = grid::X_UP;
		}
		else if (KeyTracker.IsKeyPressed(DirectX::Keyboard::A))
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Command_Click.mp3");

			if (m_loadingIndex >= 4)
				return;
			m_actions[m_loadingIndex++] = grid::X_DOWN;
		}
		else if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Q))
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Command_Click.mp3");

			if (m_loadingIndex >= 4)
				return;
			m_actions[m_loadingIndex++] = grid::STAY;
		}
		else if (KeyTracker.IsKeyPressed(DirectX::Keyboard::E))
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Command_Click.mp3");

			if (m_loadingIndex >= 4)
				return;
			m_actions[m_loadingIndex++] = grid::Y_UP;
		}
		else if (KeyTracker.IsKeyPressed(DirectX::Keyboard::X))
		{
			SoundManager::GetInstance()->PlaySound("../Resources/Sound/Effect/Command_Delete.mp3");

			if (m_loadingIndex == 0)
				return;
			m_loadingIndex--;
		}
	}
}

void PlayerObject::HandleEvent(Event* event)
{
	switch (event->eventID)
	{
	case eEventType::CHANGE_ANIMATION:
	{
		m_meshComponent->ChangeAnimation(m_playerFSM->GetAnimationName(event->GetData<const char*>()));

		std::string ChangeAniName = event->GetData<const char*>();
		ChangeAniName += "_X";
		m_uiMeshComponent->ChangeAnimation(m_playerFSM->GetAnimationName(ChangeAniName));
	}
	return;
	case eEventType::MOVE_ON_GRID:
	{
		if (event->GetData<Vector3>() == grid::STAY)
		{
			m_bstayTimer = true;
			m_bChangeTurn = event->GetData<bool>();
			return;
		}

		auto gridMovementComponent = GetComponentByTypeName<GridMovementComponent>().lock();
		gridMovementComponent->SetMoveStart(GetLocalPosition() + event->GetData<Vector3>(), event->GetData<bool>());
		m_uiMeshComponent->SetUIMove(true);
	}
	return;
	case eEventType::CRASH_ON_GRID:
	{
		auto gridMovementComponent = GetComponentByTypeName<GridMovementComponent>().lock();
		gridMovementComponent->SetMoveStart(GetLocalPosition() + event->GetData<Vector3>(), event->GetData<bool>());
	}
	return;
	case eEventType::PLAY_FIRE_PARTICLE:
	{

		m_fireParticleObject->PlayParticle(event->GetData<bool>());
	}
	return;
	default:
		return;
	}
}

void PlayerObject::ComplexCheck()
{
	PuzzleWorld* world = dynamic_cast<PuzzleWorld*>(m_pOwnerWorld.lock().get());
	assert(world);

	if (m_gridManager->IsPlayerAbove(GetLocalPosition()))
		ComplexPlayer(world->GetOtherPlayer(this));
}

void PlayerObject::ComplexPlayer(std::shared_ptr<PlayerObject> childObj)
{
	//childObj->SetParentObject(shared_from_this());
	//std::shared_ptr<SceneComponent> rootComponent = childObj->GetRootComponent().lock();
	//rootComponent->SetLocalPosition(Vector3{ 0.f, 100.f, 0.f });	// TODO : FIX

	if (!childObj->IsIdleState())
		return;

	auto childRigid = childObj->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic();
	GetComponentByTypeName<GridMovementComponent>().lock()->SetChildRigidBody(childRigid);

	m_complexed = eComplexType::PARENT;
	childObj->SetComplexType(eComplexType::CHILD);
}

void PlayerObject::UnComplexPlayer(std::shared_ptr<PlayerObject> otherObj)
{
	//ResetParentObject();
	GetComponentByTypeName<GridMovementComponent>().lock()->SetChildRigidBody(nullptr);

	//if (m_complexed == eComplexType::CHILD)
	//{
	//	//auto abc = otherObj->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic()->getGlobalPose();
	//	//GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic()->setGlobalPose(physx::PxTransform(physx::PxVec3T(abc.p.x, abc.p.y + 100.f, abc.p.z)));
	//	//m_pRootComponent->SetLocalPosition(Vector3{ otherObj->GetWorldPosition().x, otherObj->GetWorldPosition().y + 100.f, otherObj->GetWorldPosition().z });
	//}

	m_complexed = eComplexType::NOTHING;
	otherObj->SetComplexType(eComplexType::NOTHING);
}