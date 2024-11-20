#include "pch.h"
#include "TestPlayerObject.h"

#include "GridManager.h"
#include "GridMovementComponent.h"
#include "PlayerFSM.h"
#include "TestWorld.h"

#include "../Engine/SkeletalMeshComponent.h"
#include "../Engine/FSMComponent.h"
#include "../Engine/EventManager.h"
#include "../Engine/WorldManager.h"
#include "../NetworkLibrary/MyProtocol.h"
#include "../Engine/RigidDynamicComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/PointLightComponent.h"

#include "TestPlayerController.h"
#include "OnGridCollisionHandler.h"


TestPlayerObject::TestPlayerObject()
{
	InputManager::GetInstance()->AddInputProcesser(this);

	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_ANIMATION, this);
	EventManager::GetInstance()->RegisterListener(eEventType::MOVE_ON_GRID, this);
	EventManager::GetInstance()->RegisterListener(eEventType::CRASH_ON_GRID, this);

	//WorldManager::GetInstance()->RegisterHandler(S2C_SET_TURN, std::bind(&TestPlayerObject::SetTurn, this, std::placeholders::_1));
}

TestPlayerObject::~TestPlayerObject()
{
	InputManager::GetInstance()->RemoveInputProcesser(this);

	EventManager::GetInstance()->UnregisterListener(eEventType::CHANGE_ANIMATION, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::MOVE_ON_GRID, this);
	EventManager::GetInstance()->UnregisterListener(eEventType::CRASH_ON_GRID, this);

	/// GridManager
	/*m_gridManager = std::make_shared<GridManager>();
	m_gridManager->CreateMap(10, 10, 5, -900.f, 0.f, -900.f, 200.f);
	m_gridManager->SetMapState(GetGameObjects(eObjectType::LEVEL));
	std::shared_ptr<GameObject> sharedThis = shared_from_this();
	m_gridManager->SetMapState(sharedThis);*/
}

void TestPlayerObject::Initialize()
{
	m_playerFSM = std::make_shared<PlayerFSM>();

	m_meshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent").lock();
	m_meshComponent->SetDefaultAnimation(m_playerFSM->GetAnimationName("State_Idle"), true);
	m_meshComponent->AddAnimation(m_playerFSM->GetAnimationName("State_TurnWait"), false);

	if (m_bHostPlayer)
		m_meshComponent->SetLocalPosition(Vector3{ 50.f, 0.f, 50.f });
	else
		m_meshComponent->SetLocalPosition(Vector3{ 150.f, 100.f, 50.f });

	SetRootComponent(m_meshComponent);

	std::shared_ptr<FSMComponent> fsmComponent = CreateComponent<FSMComponent>("SkeletalFSMComponent").lock();
	m_playerFSM->SetOwnerObject(this);
	fsmComponent->AttachFSM(m_playerFSM);

	//auto controllerComponent = CreateComponent<ControllerComponent>("Test_Player_Controller_Component");
	//controllerComponent.lock()->CreateController<TestPlayerController>();
	CreateComponent<GridMovementComponent>("Test_Player_GridMovement_Component");
	auto CapsuleRigidBody = CreateComponent<RigidDynamicComponent>("Capsule_Rigidbody");
	CapsuleRigidBody.lock()->CreateDynamicRigidBody(RigidBodyComponent::Geometry::Capsule, { 20.f, 10.f }, { 0.5f, 0.5f, 0.0f }, 0.2f);
	CapsuleRigidBody.lock()->SetLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	CapsuleRigidBody.lock()->AddCollisionHandler<OnGridCollisionHandler>();

	auto pointLight = CreateComponent<PointLightComponent>("Test_PointLight1").lock();
	pointLight->SetRange(200);
	pointLight->SetPointLightIntencity(100);
	pointLight->SetLightColor(1, 0, 0);

	GameObject::Initialize();
}

void TestPlayerObject::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void TestPlayerObject::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	if (!m_gridManager)
		return;

	if (m_bHostPlayer && KeyTracker.IsKeyPressed(DirectX::Keyboard::I))
	{
		m_bTurn = true;
		static int hostIdx = 0;
		ProceedAction(m_hostAction[hostIdx++]);
		if (hostIdx > 6)	hostIdx = 0;
	}
	if (!m_bHostPlayer && KeyTracker.IsKeyPressed(DirectX::Keyboard::O))
	{
		m_bTurn = true;
		static int guestIdx = 0;
		ProceedAction(m_guestAction[guestIdx++]);
		if (guestIdx > 6)	guestIdx = 0;
	}
	if (KeyTracker.IsKeyPressed(DirectX::Keyboard::P))
	{
		m_bTurn = false;

		TestWorld* world = dynamic_cast<TestWorld*>(m_pOwnerWorld.lock().get());
		assert(world);
		UnComplexPlayer(world->GetOtherPlayer(this));

		EventManager::GetInstance()->SendImmediateEvent(eEventType::CHANGE_TURN, nullptr, Event_TurnInfo{ m_bHostPlayer, m_bTurn });
		EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_GROUND, nullptr, Event_GroundInfo{ m_bHostPlayer, m_gridManager->IsGround(GetWorldPosition()) });
	}
}

void TestPlayerObject::HandleEvent(Event* event)
{
	switch (event->eventID)
	{
	case eEventType::CHANGE_ANIMATION:
		m_meshComponent->ChangeAnimation(event->GetData<const char*>());
		return;
	case eEventType::MOVE_ON_GRID:
	{
		auto gridMovementComponent = GetComponentByTypeName<GridMovementComponent>().lock();
		gridMovementComponent->SetMoveStart(GetLocalPosition() + event->GetData<Vector3>(), true);
		//SetPosition(GetLocalPosition() + event->GetData<Vector3>());
	}
	return;
	case eEventType::CRASH_ON_GRID:
	{
		auto gridMovementComponent = GetComponentByTypeName<GridMovementComponent>().lock();
		gridMovementComponent->SetMoveStart(GetLocalPosition() + event->GetData<Vector3>(), true);
	}
	return;
	default:
		return;
	}
}

// 서버로부터 이번 행동 하나 받았을 때 로직 처리
void TestPlayerObject::ProceedAction(Vector3 action)
{
	// 내 상태가 Idle이라면 그라운드 체크. 내려가야 한다면 그라운드 체크 반복. TODO : 오브젝트가 Idle 상태인지 판별하는 함수
	if (m_playerFSM->IsIdleState())
		m_gridManager->FallToGround(this);
	// 합체 체크. 내 한 칸 위에 Idle 상태의 플레이어가 존재한다면 내 한 칸 위의 플레이어를 나의 자식으로 종속시킨다. TODO : 동적으로 플레이어의 부모자식 관계를 연결하고 끊을 수 있는 기능, 그리드매니저에서 위 그리드에 플레이어가 있다면 리턴
	ComplexCheck();
	// Event : 행동을 처리하기 전에 턴이 시작되었음, 이번 행동, 아래 그리드 상태를 transition에 알린다.(...->Turn_State)
	EventManager::GetInstance()->SendImmediateEvent(eEventType::CHANGE_TURN, nullptr, Event_TurnInfo{ m_bHostPlayer, m_bTurn });
	EventManager::GetInstance()->SendImmediateEvent(eEventType::GET_THIS_ACTION, nullptr, action);
	EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_GROUND, nullptr, Event_GroundInfo{ m_bHostPlayer, m_gridManager->IsGround(GetWorldPosition()) });
	// 행동을 처리한다. 합체 상태라면 상승, 하강 경계치 조정을 잘 해야한다. TODO : 합체 상태라면 자식 플레이어의 움직임도 같이 처리해야 한다.
	m_gridManager->MoveOnGrid(this, action);
	// Event : 행동을 처리한 후 턴이 끝났음, 아래 그리드 상태를 transition에 알린다.(Turn_State->...)
	//EventManager::GetInstance()->SendImmediateEvent(eEventType::CHANGE_TURN, false);
	//EventManager::GetInstance()->SendImmediateEvent(eEventType::CHECK_GROUND, m_gridManager->IsGround(GetLocalPosition()));
	// 만약 Idle 상태라면 그라운드 체크를 한 번 더 한다.
	//FallToGround();
	// TODO : 그라운드 체크, 합체 체크하는 함수를 private으로 하나 뺀다.
}

void TestPlayerObject::FallToGround()
{
	m_gridManager->FallToGround(this);
}

void TestPlayerObject::ComplexCheck()
{
	TestWorld* world = dynamic_cast<TestWorld*>(m_pOwnerWorld.lock().get());
	assert(world);

	if (m_gridManager->IsPlayerAbove(GetLocalPosition()))
		ComplexPlayer(world->GetOtherPlayer(this));
}

// 부모 플레이어에서 호출
void TestPlayerObject::ComplexPlayer(std::shared_ptr<TestPlayerObject> childObj)
{
	//childObj->SetParentObject(shared_from_this());

	auto childRigid = childObj->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic();
	GetComponentByTypeName<GridMovementComponent>().lock()->SetChildRigidBody(childRigid);
/*	std::shared_ptr<SceneComponent> rootComponent = childObj->GetRootComponent().lock();
	rootComponent->SetLocalPosition(Vector3{ 0.f, 100.f, 0.f });*/	// TODO : FIX

	//auto abc = childObj->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic()->getGlobalPose();
	//childObj->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic()->setGlobalPose(physx::PxTransform(physx::PxVec3T(abc.p.x, abc.p.y + 100.f, abc.p.z)));

	m_complexed = eComplexType::PARENT;
	childObj->SetComplexType(eComplexType::CHILD);
}

void TestPlayerObject::UnComplexPlayer(std::shared_ptr<TestPlayerObject> otherObj)
{
	//ResetParentObject();
	GetComponentByTypeName<GridMovementComponent>().lock()->SetChildRigidBody(nullptr);

	if (m_complexed == eComplexType::CHILD)
	{
		//auto abc = otherObj->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic()->getGlobalPose();
		//GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic()->setGlobalPose(physx::PxTransform(physx::PxVec3T(abc.p.x, abc.p.y + 100.f, abc.p.z)));
		//m_pRootComponent->SetLocalPosition(Vector3{ otherObj->GetWorldPosition().x, otherObj->GetWorldPosition().y + 100.f, otherObj->GetWorldPosition().z });
	}

	m_complexed = eComplexType::NOTHING;
}

void TestPlayerObject::Network_ProceedAction()
{
	if (m_bHostPlayer)		// host가 움직
	{
		//PacketC2S_CharacterMove cm;
		//cm.who = '0'; // host가 움직이려함.
		//cm.direction = '0'; // 일단 Z_UP

		//WorldManager::GetInstance()->PushSendQueue(
		//	WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_CharacterMove), C2S_CHARACTER_MOVE, &cm.who),
		//	sizeof(PacketC2S_CharacterMove));
	}

	else if (!m_bHostPlayer)	// guest가 움직
	{
		//PacketC2S_CharacterMove cm;
		//cm.who = '1'; // guest가 움직이려함.
		//cm.direction = '0'; // 일단 Z_DOWN

		//WorldManager::GetInstance()->PushSendQueue(
		//	WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_CharacterMove), C2S_CHARACTER_MOVE, &cm.who),
		//	sizeof(PacketC2S_CharacterMove));
	}
	m_index++;
}

void TestPlayerObject::Network_CheckAction()
{
	// 끝내기
	if (m_index >= 4)
	{
		WorldManager::GetInstance()->PushSendQueue(
			WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_EndAction), C2S_END_ACTION, nullptr),
			sizeof(PacketC2S_EndAction));
	}
}
void TestPlayerObject::Network_ChangeTurn()
{
	// 턴 체인지 해주기
	PacketC2S_SetTurn st;

	if (m_bTurn)
	{
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
}
