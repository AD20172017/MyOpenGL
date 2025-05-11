#pragma once
#include <memory>
//#include <Windows.h>
//宏
#define CPP_CORE_BEGIN namespace Core {
#define CPP_CORE_END }
#define BIT(x) (1 << x)

CPP_CORE_BEGIN

//模板
template <typename T>
using Ptr = std::shared_ptr<T>;

template <typename T,typename ... Args>
constexpr Ptr<T> CreatePtr(Args&& ... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}



CPP_CORE_END

//头文件
#include "Core/Log.h"