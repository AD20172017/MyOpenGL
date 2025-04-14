#pragma once
#include <iostream>
#include <assert.h>
#include <functional>
#include <memory>

#define GLRT_BEGIN namespace GLRT {
#define GLRT_END }

namespace GLRT {

#define LOG(OUT,x) OUT << x << std::endl
#define LOG_ERROR(OUT,x) OUT << "\n\nError: " << x << std::endl
#define LOG_WARNING(OUT,x) OUT << "\n\nWarning: " << x << std::endl
#define LOG_INFO(OUT,x) OUT << "\n\nInfo: " << x << std::endl

// Fix for E0052: 应输入宏参数名
#ifdef _DEBUG
#define LOG_DEBUG(OUT,x) OUT << "\n\nDebug: " << x << std::endl
#else
#define LOG_DEBUG(OUT,x)
#endif // _DEBUG

// 断言
#define ASSERT(cond,msg)        \
if(!(cond))                        \
{                                  \
LOG_ERROR(std::cerr,msg);				   \
assert(cond);                  \
}

#define SAFE_DELETE(p)do{if(p) delete p;p=nullptr;}while(0)
#define SAFE_RELEASE(p)do{if(p) p->release();}while(0)
#define SAFE_RELEASE_NULL(p)do{if(p) p->release(); p = nullptr;}while(0)
#define SAFE_RETAIN(p) do { if(p) { (p)->retain(); } } while(0)
#define BREAK_IF(cond) if(cond) break

}