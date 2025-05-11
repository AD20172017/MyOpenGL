#pragma once
#include <iostream>
#include <assert.h>
#include <functional>
#include "Core/Log.h"
#define GLRT_BEGIN namespace GLRT {
#define GLRT_END }

namespace GLRT {

#define LOG_TRACE(...) CLI_TRACE(__VA_ARGS__)
#define LOG_ERROR(...) CLI_ERROR(__VA_ARGS__)
#define LOG_WARN(...) CLI_WARN(__VA_ARGS__)
#define LOG_INFO(...) CLI_INFO(__VA_ARGS__)
#define LOG_CRITICAL(...) CLI_CRITICAL(__VA_ARGS__)


// Fix for E0052: 应输入宏参数名
#ifdef _DEBUG
#define LOG_DEBUG(...) LOG_TRACE(__VA_ARGS__)
#else
#define LOG_DEBUG(OUT,x)
#endif // _DEBUG

// 断言
#define ASSERT(cond,...)        \
if(!(cond))                        \
{                                  \
LOG_ERROR(__VA_ARGS__);				   \
assert(cond);                  \
}
/*
#define SAFE_DELETE(p)do{if(p) delete p;p=nullptr;}while(0)
#define SAFE_RELEASE(p)do{if(p) p->release();}while(0)
#define SAFE_RELEASE_NULL(p)do{if(p) p->release(); p = nullptr;}while(0)
#define SAFE_RETAIN(p) do { if(p) { (p)->retain(); } } while(0)
#define BREAK_IF(cond) if(cond) break
*/

}