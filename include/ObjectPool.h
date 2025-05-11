#pragma once
//#include "Object.h"
#include "Marcos.h"
#include "Singleton.h"
#include <unordered_map>
#include <queue>
#include <string>
GLRT_BEGIN


// ObjectPool独享object对象的创建权，在object中声明友元来访问object接口的构造函数
//1.基类中声明友元来访问object的并创建接口
//2.工厂方法，静态工厂方法
//3.克隆原型
//4.子类声明objectpool
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

