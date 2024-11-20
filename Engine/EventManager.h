#pragma once

#include "Singleton.h"
#include "Event.h"
#include "EventListener.h"

#include <list>
#include <map>


enum class eEventType
{
	CHANGE_WORLD,
	GO_TO_PREV_WORLD,
	DELETE_OBJECT,

	// PlayerObject > Transition 전달 정보
	CHANGE_TURN,
	GET_THIS_ACTION,
	CHECK_GROUND,
	CHECK_CRASH,
	CHECK_GET_ITEM,

	// State > SkeletalMeshComponent 전달 정보
	CHANGE_ANIMATION,

	// 그리드 무브먼트
	MOVE_ON_GRID,
	CRASH_ON_GRID,

	PLAY_CRASH_PARTICLE,
	PLAY_FIRE_PARTICLE,

	SET_PRE_STATE,

	END
};

class EventListener;

/// <summary>
///	1. RegisterListener()로 이벤트타입과 리스너를 등록
///	2. SendEvent()로 이벤트를 생성하고 EventManager.Run()에서 실행할 m_EventList에 추가
///	3. Run()에서 이벤트 Execute
/// </summary>
class EventManager
	:public Singleton<EventManager>
{
public:
	// 해당 eventType으로 listener 등록, 해제
	void RegisterListener(eEventType eventType, EventListener* listener);
	void UnregisterListener(eEventType eventType, EventListener* listener);

	// 해당 listener에 등록된 모든 이벤트 해제
	void UnregisterAll(EventListener* listener);


	void Initialize();
	void Run();
	void Finalize();

	/// 리스너로 등록된 모든 객체가 이벤트를 실행하기 원한다면 EventListener를 nullptr로 Send
	template <typename... Args>
	void SendEvent(eEventType eventId, EventListener* listener, Args... args)
	{
		Event newEvent(args...);
		newEvent.eventID = eventId;
		newEvent.eventListener = listener;

		m_eventList.emplace_back(newEvent);
	}

	template <typename... Args>
	void SendImmediateEvent(eEventType eventId, EventListener* listener, Args... args)
	{
		Event* event = new Event(args...);
		event->eventID = eventId;

		if (listener)
			listener->HandleEvent(event);

		std::pair<std::multimap<eEventType, EventListener*>::iterator,
			std::multimap<eEventType, EventListener*>::iterator> range;
		range = m_wholeEvents.equal_range(event->eventID);

		for (auto& iter = range.first; iter != range.second; ++iter)
		{
			// 해당 객체의 HandleEvent 함수 실행
			iter->second->HandleEvent(event);
		}

		delete event;
	}

private:
	bool IsRegistered(eEventType eventType, EventListener* listener);
	void ExecuteEvent(Event* event);


private:
	std::multimap<eEventType, EventListener*> m_wholeEvents;	//  해당 타입으로 등록된 구독자 목록
	std::list<Event> m_eventList;	// 처리되어야 하는 이벤트 큐
};

