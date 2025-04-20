#pragma once

#include "Event.h"
#include "Types.h"
#include <functional>
#include <list>
#include <unordered_map>

ECS_BEGIN
class EventManager
{
public:
	void AddListener(EventType EventType, std::function<void(Event&)> const& listener)
	{
		listeners[EventType].push_back(listener);
	}

	void SendEvent(Event& event)
	{
		uint32_t type = event.GetType();

		for (auto const& listener : listeners[type])
		{
			listener(event);
		}
	}

	void SendEvent(EventType EventType)
	{
		Event event(EventType);

		for (auto const& listener : listeners[EventType])
		{
			listener(event);
		}
	}

private:
	std::unordered_map<EventType, std::list<std::function<void(Event&)>>> listeners;
};

ECS_END

