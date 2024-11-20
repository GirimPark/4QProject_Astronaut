#pragma once

class FSM;
class FSMTransition;

/// <summary>
///	인스턴스화할 FSMState의 원형
///	전환 조건들을 가지고 있으며, 가진 전환 조건들을 체크한다.
///	State이름은 State_로 시작하도록 명명한다.
/// </summary>
class FSMState
{
public:
	FSMState() = default;
	virtual ~FSMState() = default;


	std::string GetName() { return m_name; }

	void SetName(std::string_view name) { m_name = name; }
	void SetParentFSM(FSM* parentFSM) { m_pParentFSM = parentFSM; }


	void AddTransition(FSMTransition* transition);

	virtual bool CheckTransition();

	virtual void OnEnter() abstract;
	virtual void OnExit() abstract;

protected:
	virtual void DecideFSMType() abstract;

protected:
	FSM* m_pParentFSM;

	std::string m_name;
	std::vector<FSMTransition*> m_transitions;
};