#pragma once
#include "Marcos.h"
#include <memory>
#include <functional>
#include <ObjectPool.h>
#include <iostream>
#include <Windows.h>
GLRT_BEGIN
//使用时需注意最后一行需要加public
//外部调用使用GLASS::Ptr
//object类不能自己new出来
#define OBJECT_SETUP(CLASS) \
public:\
	typedef std::shared_ptr<CLASS> Ptr;\
	typedef std::shared_ptr<const CLASS> CPtr;\
	typedef CLASS TYPE_NAME_FOR_OBJECTPOOL;\
	virtual const char * GetClassName(){ return #CLASS; }\
	Ptr This(){ return std::dynamic_pointer_cast<CLASS>(shared_from_this()); }\
	CPtr CThis() const { return std::dynamic_pointer_cast<const CLASS>(shared_from_this()); }\
	friend class ObjectPool;\
	virtual ~CLASS() = default;\
protected:\
	CLASS()=default;\
private:\
	virtual Ptr create(CLASS* V,const std::function<void(Object*)> & lambda) { return Ptr(V, lambda);}

//还是不要写死
//virtual Ptr create(const std::function<void()>& lambda) { return std::make_shared<CLASS>(new CLASS(), [lambda](CLASS* obj) { ObjectPool::GetInstance()->release(obj)}); }


//PS:要想硬性保证成员函数只被一个友元类访问,该成员函数的普通类也不能访问那就只能声明static或者委托,实在太麻烦了,以上的create不能硬性保证,需要自己自觉


//ObjectPool管理的对象使用一下命令创建

#define ACQUIRE_FUNC(__TYPE__)                        \
static std::shared_ptr<__TYPE__> acquire()            \
{                                          \
	__TYPE__* p=new __TYPE__(); \
	std::shared_ptr<__TYPE__> pRet = std::dynamic_pointer_cast<__TYPE__>(ObjectPool::GetInstance()->acquire(p));     \
	if(!(pRet&&pRet->init())){std::cout<<" Acquire failed!! CLASS: "<<(#__TYPE__)<<std::endl;pRet.reset();}							   \
	return pRet;								 \
}




template <typename T>
std::shared_ptr<T> ToPtr(T* op) {
	return std::shared_ptr<T>(op, T::ProtectedDelete);
}

template <typename T>
std::shared_ptr<const T> ToCPtr(const T* op) {
	return std::shared_ptr<const T>(op, T::ProtectedDelete);
}
//子类的构造函数为protected,即不能外部new出来,只能使用ObjectPool::acquire()来创建
class Object :public std::enable_shared_from_this<Object>
{
	OBJECT_SETUP(Object)
public:

	ACQUIRE_FUNC(Object)

		//友元函数
	template <typename T>
	friend std::shared_ptr<T> ToPtr(T* op);
	template <typename T>
	friend std::shared_ptr<const T> ToCPtr(const T* op);

	//子类能被ObjectPool访问并创建吗
	//friend class ObjectPool;


	//CREATE_FUNC(Object);

	virtual bool init() { return true; }
	//保留
	//void retain();
	//释放
	//void release();

	virtual bool clear() { return true; }


	//友元
	//friend class AutoreleasePool;

	//virtual ~Object() {}
protected:

	static void ProtectedDelete(const Object* op) {
		delete op;
	}
	//声明命名空间
	using std::enable_shared_from_this<Object>::shared_from_this;
	bool isValid=false;




};


GLRT_END
