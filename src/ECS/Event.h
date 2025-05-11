#pragma once
#include "Types.h"
#include <any>
#include <unordered_set>
ECS_BEGIN

class Event {
public:
	Event() = delete;
	
	explicit Event(EventType type) : m_type(type) {}

	template<typename T>
	void SetParam(EventType type, T value)
	{
		m_data = value;
	}

	template<typename T>
	T GetParam(EventType type)
	{
		return std::any_cast<T>(m_data);
	}

	EventType GetType() const
	{
		return m_type;
	}

private:
	EventType m_type{};
	std::any m_data{};
};


ECS_END

