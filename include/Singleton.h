#pragma once
#include <memory>
#include<mutex>

namespace GLRT {
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
}