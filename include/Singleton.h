#pragma once
#include <memory>
#include<mutex>
//到底是直接用静态局部变量,还是new出来再用静态指针?new出来的对象貌似不主动delete不会调用析构
namespace GLRT {

//不需要延迟初始的情况
	template<typename T>
	class Singleton
	{
	protected:
		Singleton() = default;
		virtual~Singleton() = default;
	public:
		static T* GetInstance()
		{
			static T instance;
			return &instance;
		}
	};
}

/*
template<typename T>
	class Singleton
	{
	protected:
		Singleton() = default;
		virtual~Singleton() = default;
		inline static T* instance = nullptr;
	public:
		static T* GetInstance()
		{
			static std::once_flag flag;
			std::call_once(flag, [&]() {
				instance=new T();
				});
			return instance;
		}
	};

*/