#pragma once

/// <summary>
///	인스턴스화할 FSMTransition의 원형
///	전환 조건을 해당 Functor로 구현한다.
///	</summary>
class FSMTransition
{
public:
	virtual std::string operator()() abstract;
};