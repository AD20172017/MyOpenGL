#include "ObjectPool.h"
#include <string>
#include <Object.h>

GLRT_BEGIN
ObjectPool::ObjectPool() {
	::Core::Log::init();
	LOG_TRACE("ObjectPool");
}

std::shared_ptr<Object> ObjectPool::acquire(Object* obj) {
	ASSERT(obj, "ObjectPool::acquire(): obj is nullptr");
	auto it = m_objectPool.find(obj->GetClassName());
	if (it == m_objectPool.end()) {
		m_objectPool[obj->GetClassName()] = std::queue<Object*>();
	}
	auto& queue = m_objectPool[obj->GetClassName()];
	std::shared_ptr<Object> objPtr;
	if (queue.empty()) {
		objPtr = obj->create(obj, [this](Object* obj) { release(obj); });
	}
	else {
		objPtr = std::shared_ptr<Object>(queue.front(), [this](Object* obj) { release(obj); });
		queue.pop();
	}
	objPtr->isValid = true;
	return objPtr;
}

void ObjectPool::release(Object* obj) {
	ASSERT(obj, "ObjectPool::release(): obj is nullptr");
	ASSERT(obj->clear(), "ObjectPool::release(): obj clear failed");
	auto name = obj->GetClassName();
	auto& queue = m_objectPool[name];
	queue.push(obj);
	LOG_DEBUG("ObjectPool::release(): CLASS:{0}", name);
}


void ObjectPool::clearPool() {

	for (auto& [name, queue] : m_objectPool) {
		while (!queue.empty()) {
			Object* tmp = queue.front();
			queue.pop();
			if (tmp == nullptr) {
				continue;
			}
			delete tmp;
			LOG_DEBUG("ObjectPool::clearPool(): CLASS:{0}", name);
		}
	}
	m_objectPool.clear();
}
//析构貌似是不可控的
ObjectPool::~ObjectPool() {
	LOG_DEBUG( "ObjectPool::~ObjectPool()");
	this->clearPool();
}

GLRT_END
