#pragma once
//#include "Object.h"
#include "Marcos.h"
#include "Singleton.h"
#include <unordered_map>
#include <queue>
#include <string>
GLRT_BEGIN


// ObjectPool����object����Ĵ���Ȩ����object��������Ԫ������object�ӿڵĹ��캯��
//1.������������Ԫ������object�Ĳ������ӿ�
//2.������������̬��������
//3.��¡ԭ��
//4.��������objectpool
class Object;

class ObjectPool : public Singleton<ObjectPool>
{
	//OBJECT_SETUP(ObjectPool)
private:
	std::unordered_map<const char*, std::queue<Object*>> m_objectPool;

	/*std::unordered_map<const char*, std::function<std::shared_ptr<Object>()>> factories;
	template <typename T>
	void registerFactory() {
		static bool flag = false;
		if (flag)return;
		flag = true;
		auto factory = []() -> std::shared_ptr<Object> {
			return std::make_shared<T>();
			};
		factories[T().GetClassName] = factory;
	}*/


public:
	ObjectPool();
	virtual ~ObjectPool();

	std::shared_ptr<Object> acquire(Object* obj);

	void release(Object* obj);

	void clearPool();


};

GLRT_END

