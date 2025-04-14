#include "ObjectPool.h"
#include <string>
#include <Object.h>

GLRT_BEGIN

std::shared_ptr<Object> ObjectPool::acquire(Object* obj) {
   auto it = m_objectPool.find(obj->GetClassName());
   if (it == m_objectPool.end()) {
       m_objectPool[obj->GetClassName()] = std::queue<Object*>();
   }
   auto& queue = m_objectPool[obj->GetClassName()];

   if (queue.empty()) {
       return obj->create(obj,[this](Object* obj) { release(obj); });
   } else {
       std::shared_ptr<Object> obj(queue.front(), [this](Object* obj) { release(obj); });
       queue.pop();
       return obj;
   }
}

void ObjectPool::release(Object* obj) {
   auto& queue = m_objectPool[obj->GetClassName()];
   queue.push(obj);
}


void ObjectPool::clearPool() {
	static std::string clearStr="cleared";
	for (auto& [name, queue] : m_objectPool) {
		while (!queue.empty()) {
			delete queue.front();
			queue.pop();
		}
		LOG(std::cout, std::string(name) + clearStr);
	}
}

ObjectPool::~ObjectPool() {
	this->clearPool();
}

GLRT_END
