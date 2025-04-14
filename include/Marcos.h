#pragma once
#include <iostream>
#include <assert.h>
#include <functional>
#include <memory>

#define GLRT_BEGIN namespace GLRT {
#define GLRT_END }



namespace GLRT {

#define LOG(OUT,x) OUT << x << std::endl
#define LOG_ERROR(OUT,x) OUT << "Error: " << x << std::endl
#define LOG_WARNING(OUT,x) OUT << "Warning: " << x << std::endl
#define LOG_INFO(OUT,x) OUT << "Info: " << x << std::endl

	//╤оят
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