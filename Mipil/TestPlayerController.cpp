#include "pch.h"
#include "TestPlayerController.h"
#include "../Engine/RigidDynamicComponent.h"
#include "../Engine/EventManager.h"

#include "GridManager.h"
#include "TestWorld.h"
#include "TestPlayerObject.h"

void TestPlayerController::HandleInput(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	//if (m_player == nullptr)
	//{
	//	m_player = dynamic_cast<TestPlayerObject*>(m_owner.lock().get());
	//}

	//if (m_world == nullptr)
	//{
	//	m_world = dynamic_cast<TestWorld*>(m_player->GetOwnerWorld().lock().get());
	//}

	if (m_rigidDynamic == nullptr)
	{
		m_rigidDynamic = m_owner.lock()->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic();
	}

	//if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Enter))
	//{
	//	m_player->SetTurn(!m_player->GetTurn());
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetTurn());
	//}

	//if (KeyTracker.IsKeyPressed(DirectX::Keyboard::W))
	//{
	//	m_player->GetGridManager().lock()->MoveOnGrid(m_player, grid::Z_UP);
	//	EventManager::GetInstance()->SendEvent(eEventType::GET_THIS_ACTION, nullptr, grid::Z_UP);
	//	m_player->SetTurn(true);
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetTurn());
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetGridManager().lock()->IsGround(m_player->GetPosition()));
	//}
	//if (KeyTracker.IsKeyPressed(DirectX::Keyboard::S))
	//{
	//	m_player->GetGridManager().lock()->MoveOnGrid(m_player, grid::Z_DOWN);
	//	EventManager::GetInstance()->SendEvent(eEventType::GET_THIS_ACTION, nullptr, grid::Z_DOWN);
	//	m_player->SetTurn(true);
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetTurn());
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetGridManager().lock()->IsGround(m_player->GetPosition()));
	//}
	//if (KeyTracker.IsKeyPressed(DirectX::Keyboard::D))
	//{
	//	m_player->GetGridManager().lock()->MoveOnGrid(m_player, grid::X_UP);
	//	EventManager::GetInstance()->SendEvent(eEventType::GET_THIS_ACTION, nullptr, grid::X_UP);
	//	m_player->SetTurn(true);
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetTurn());
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetGridManager().lock()->IsGround(m_player->GetPosition()));
	//}
	//if (KeyTracker.IsKeyPressed(DirectX::Keyboard::A))
	//{
	//	m_player->GetGridManager().lock()->MoveOnGrid(m_player, grid::X_DOWN);
	//	EventManager::GetInstance()->SendEvent(eEventType::GET_THIS_ACTION, nullptr, grid::X_DOWN);
	//	m_player->SetTurn(true);
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetTurn());
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetGridManager().lock()->IsGround(m_player->GetPosition()));
	//}
	//if (KeyTracker.IsKeyPressed(DirectX::Keyboard::Q))
	//{
	//	m_player->GetGridManager().lock()->MoveOnGrid(m_player, grid::Y_UP);
	//	EventManager::GetInstance()->SendEvent(eEventType::GET_THIS_ACTION, nullptr, grid::Y_UP);
	//	m_player->SetTurn(true);
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetTurn());
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetGridManager().lock()->IsGround(m_player->GetPosition()));
	//}
	//if (KeyTracker.IsKeyPressed(DirectX::Keyboard::E))
	//{
	//	m_player->GetGridManager().lock()->MoveOnGrid(m_player, grid::STAY);
	//	EventManager::GetInstance()->SendEvent(eEventType::GET_THIS_ACTION, nullptr, grid::STAY);
	//	m_player->SetTurn(true);
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetTurn());
	//	EventManager::GetInstance()->SendEvent(eEventType::CHANGE_TURN, nullptr, m_player->GetGridManager().lock()->IsGround(m_player->GetPosition()));
	//}



	if (KeyTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		m_rigidDynamic->addForce(PxVec3(0.0f, 10000.f, 0.0f), PxForceMode::eIMPULSE);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::Up))
	{
		m_rigidDynamic->addForce(PxVec3(0.0f, 0.f, .2f), PxForceMode::eVELOCITY_CHANGE);
	}

	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::Down))
	{
		m_rigidDynamic->addForce(PxVec3(0.0f, 0.f, -.2f), PxForceMode::eVELOCITY_CHANGE);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::Right))
	{
		m_rigidDynamic->addForce(PxVec3(.2f, 0.f, 0.0f), PxForceMode::eVELOCITY_CHANGE);
	}

	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::Left))
	{
		m_rigidDynamic->addForce(PxVec3(-.2f, 0.f, 0.0f), PxForceMode::eVELOCITY_CHANGE);
	}


}

void TestPlayerController::Update(float deltaTime)
{
}
