#pragma once

class Event;

class EventListener
{
public:
	virtual ~EventListener() = default;

	virtual void HandleEvent(Event* event) abstract;
};