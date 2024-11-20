#pragma once
#include <directXTK/Mouse.h>
#include <directXTK/Keyboard.h>
#include "GameObject.h"

using namespace DirectX;
/// <summary>
/// Controller
/// 이거 상속시켜서 각각 용도에 맞는 인풋 처리 로직 구현 해주면 된다~
/// HandleInput()에 각 키 입력에 따른 변동 사항 처리하고,
/// Run()에서 그 처리된 데이터로 업데이트 해줄거 업데이트 해주면
/// HandleInput()은 인풋매니저 업데이트 될 때 처리되고,
/// Run()는 월드->게임 오브젝트->ControllerComponent 업데이트 될 때 호출된다.
/// 여기서 처리된 데이터로 GameObject에 영향주기 쉽게 하려고
/// ControllerComponent->CreateController() 할 때 m_owner에다
/// ControllerComponent의 주인 GameObject 넣어줌. 이걸로 접근해서 처리하셈.
/// </summary>

class Controller
{
public:
	virtual void HandleInput(
		const Keyboard::State& KeyState,
		const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState,
		const Mouse::ButtonStateTracker& MouseTracker) abstract;
	virtual void Update(float deltaTime) abstract;

	void SetOwner(std::weak_ptr<GameObject> owner) { m_owner = owner; }

protected:
	std::weak_ptr<GameObject> m_owner;
};

