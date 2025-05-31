#pragma once
#include "Core/Event.h"
#include "Core/KeyCodes.h"
#include <sstream>
#include "Object.h"
using namespace GLRT;
CPP_CORE_BEGIN
//using namespace Key;
class KeyEvent : public IEvent
{
public:
	KeyCode GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(const KeyCode keycode)
		: m_KeyCode(keycode) {
	}

	KeyCode m_KeyCode;
};


//实例类
class KeyPressedEvent : public KeyEvent, public Object
{
	OBJECT_SETUP(KeyPressedEvent)
public:
	static std::shared_ptr<KeyPressedEvent> acquire(const KeyCode keycode, bool isRepeat = false) {
		KeyPressedEvent* p = new KeyPressedEvent(keycode, isRepeat);
		std::shared_ptr<KeyPressedEvent> pRet = std::dynamic_pointer_cast< KeyPressedEvent >( ObjectPool::GetInstance()->acquire(p) );
		if ( !( pRet && pRet->init() ) ) {
			std::cout << " Acquire failed!! CLASS: " << ( "KeyPressedEvent" ) << std::endl; pRet.reset();
		}
		return pRet;
	}
private:
	KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
		: KeyEvent(keycode), m_IsRepeat(isRepeat) {
	}
public:

	bool IsRepeat() const { return m_IsRepeat; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
private:
	bool m_IsRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const KeyCode keycode)
		: KeyEvent(keycode) {
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(const KeyCode keycode)
		: KeyEvent(keycode) {
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};

CPP_CORE_END