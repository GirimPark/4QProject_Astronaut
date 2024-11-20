#include "pch.h"
#include "EventManager.h"

void EventManager::RegisterListener(eEventType eventType, EventListener* listener)
{
	// listener nullptr 확인, 이미 등록되어 있는지 확인
	assert(listener);
	if (IsRegistered(eventType, listener))
	{
		return;
	}

	m_wholeEvents.emplace(std::make_pair(eventType, listener));
}

void EventManager::UnregisterListener(eEventType eventType, EventListener* listener)
{
	std::pair<std::multimap<eEventType, EventListener*>::iterator,
		std::multimap<eEventType, EventListener*>::iterator> range;
	range = m_wholeEvents.equal_range(eventType);

	for (auto& iter = range.first; iter != range.second; ++iter)
	{
		if (iter->second == listener)
		{
			iter = m_wholeEvents.erase(iter);
			break;
		}
	}
}

void EventManager::UnregisterAll(EventListener* listener)
{
	auto iter = m_wholeEvents.begin();

	while (iter != m_wholeEvents.end())
	{
		if (iter->second == listener)
		{
			iter = m_wholeEvents.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void EventManager::Initialize()
{

}

void EventManager::Run()
{
	while (!m_eventList.empty())
	{
		ExecuteEvent(&m_eventList.front());
		m_eventList.pop_front();
	}
}

void EventManager::Finalize()
{
	m_wholeEvents.clear();
	m_eventList.clear();

	Singleton<EventManager>::Finalize();
}

bool EventManager::IsRegistered(eEventType eventType, EventListener* listener)
{
	bool rt = false;

	// 해당 이벤트 타입을 키값으로 가진 최소 범위, 최대 범위를 반환
	// 탐색 범위 축소
	std::pair<std::multimap<eEventType, EventListener*>::iterator,
		std::multimap<eEventType, EventListener*>::iterator> range;
	range = m_wholeEvents.equal_range(eventType);

	// 이미 등록된 이벤트인지 확인
	for (auto& iter = range.first; iter != range.second; ++iter)
	{
		if (iter->second == listener)
		{
			rt = true;
			break;
		}
	}

	return rt;
}

void EventManager::ExecuteEvent(Event* event)
{
	if(event->eventListener)
	{
		event->eventListener->HandleEvent(event);
		return;
	}

	std::pair<std::multimap<eEventType, EventListener*>::iterator,
		std::multimap<eEventType, EventListener*>::iterator> range;
	range = m_wholeEvents.equal_range(event->eventID);

	for (auto& iter = range.first; iter != range.second; ++iter)
	{
		// 해당 객체의 HandleEvent 함수 실행
		iter->second->HandleEvent(event);
	}
}
