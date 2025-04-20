#include "ObjectPool.h"
#include <string>
#include <Object.h>

GLRT_BEGIN
ObjectPool::ObjectPool() {
	LOG(std::cout, "ObjectPool");
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
		objPtr=obj->create(obj, [this](Object* obj) { release(obj); });
	}
	else {
		objPtr= std::shared_ptr<Object>(queue.front(), [this](Object* obj) { release(obj); });
		queue.pop();
	}
	objPtr->isValid = true;
	return objPtr;
}

void ObjectPool::release(Object* obj) {
	ASSERT(obj, "ObjectPool::release(): obj is nullptr");
	ASSERT(obj->clear(), "ObjectPool::release(): obj clear failed");
	auto name = obj->GetClassName();
	LOG_DEBUG(std::cout, "\nObjectPool::release():\nrelease CLASS: " << name);
	auto& queue = m_objectPool[name];
	queue.push(obj);
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
		}
		LOG_DEBUG(std::cout, "\nObjectPool::clearPool():\nCleared CLASS:" << name);
	}
	m_objectPool.clear();
}

ObjectPool::~ObjectPool() {
	LOG_DEBUG(std::cout, "\nObjectPool::~ObjectPool()\n");
	this->clearPool();
}

GLRT_END
