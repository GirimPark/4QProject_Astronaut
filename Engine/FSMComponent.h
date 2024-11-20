#pragma once
#include "Component.h"

class FSM;

/// <summary>
/// FSM을 오브젝트에 붙이기 위한 Component
/// FSM 구조는 다음과 같다.
/// GameObject
/// -FSMComponent(!!)
///  -FSM
///   -FSMState
///    -FSMTransition
/// 
/// EventManager를 통해서 정보를 받아오고 조작한다.
/// </summary>
class FSMComponent
	: public Component
{
public:
	void AttachFSM(std::shared_ptr<FSM> FSM);

	void Initialize() final;
	void Update(float deltaTime) final;

private:
	std::shared_ptr<FSM> m_FSM;
};

