#pragma once
#include "../Engine/EventListener.h"
#include "../Engine/GameObject.h"
#include "../Engine/InputManager.h"

#include "PlayerObject.h"

class GridManager;
class PlayerFSM;
class SkeletalMeshComponent;
class FSMComponent;

// 플레이어 입력을 받을 때 상하좌우/상승/가만히로 받는데
// 액션 타입을 따로 둘 것인지, grid::이걸로 퉁칠건지
// 나중에 필요하면 액션 타입 따로 두는걸로
//enum class eActionType
//{
//	WAIT,
//	MOVE,
//	UP,
//
//	END
//};

//enum class eComplexType
//{
//	NOTHING,	// 합체되지 않은 상태
//	PARENT,		// 합체된 부모 플레이어인 경우
//	CHILD,		// 합체된 자식 플레이어인 경우
//
//	END
//};

class TestPlayerObject
	: public GameObject
	, public EventListener
	, public InputProcesser
{
public:
	TestPlayerObject();
	~TestPlayerObject();

	bool GetHostPlayer() const { return m_bHostPlayer; }
	bool GetTurn() const { return m_bTurn; }
	std::weak_ptr<GridManager> GetGridManager() { return m_gridManager; }

	eComplexType GetComplexType() const { return m_complexed; }

	void SetHostPlayer(bool isHost) { m_bHostPlayer = isHost; }
	void SetTurn(bool isTurn) { m_bTurn = isTurn; }
	void SetComplexType(eComplexType complexed) { m_complexed = complexed; }
	void SetGridManager(std::shared_ptr<GridManager> gridManager) { m_gridManager = gridManager; }

	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;

	void HandleEvent(Event* event) override;

// 게임 로직
public:
	void ProceedAction(Vector3 action);	// 서버로부터 이번 행동 하나 받았을 때 로직 처리

	void Network_ProceedAction();
	void Network_CheckAction();
	void Network_ChangeTurn();

	void FallToGround();	// FSM State에서 호출하기 위한 용도

private:
	void ComplexCheck();

	void ComplexPlayer(std::shared_ptr<TestPlayerObject> childObj);	// 부모 플레이어에서 호출
	void UnComplexPlayer(std::shared_ptr<TestPlayerObject> otherObj);

private:
	std::shared_ptr<SkeletalMeshComponent> m_meshComponent;
	std::shared_ptr<PlayerFSM> m_playerFSM;
	std::shared_ptr<GridManager> m_gridManager;

	// 호스트 서버의 플레이어인지 아닌지
	bool m_bHostPlayer = false;
	// host 턴인지 클라턴인지 확인
	bool m_bTurn = false;

	eComplexType m_complexed = eComplexType::NOTHING;

	Vector3 m_action[4] = { grid::Z_UP, grid::Z_UP ,grid::Z_UP ,grid::Z_UP };

	Vector3 m_hostAction[6] = { grid::X_UP, grid::Z_UP ,grid::Y_UP ,grid::X_UP, grid::Y_UP, grid::Z_DOWN };
	Vector3 m_guestAction[6] = { grid::STAY, grid::STAY, grid::STAY, grid::STAY, grid::STAY, grid::STAY };

	int m_index = 0;





};