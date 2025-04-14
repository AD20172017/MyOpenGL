#pragma once
#include "Marcos.h"
#include <memory>
#include <functional>
#include <ObjectPool.h>
#include <iostream>

GLRT_BEGIN
//ʹ��ʱ��ע�����һ����Ҫ��public
//�ⲿ����ʹ��GLASS::Ptr
//object�಻���Լ�new����
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

//���ǲ�Ҫд��
//virtual Ptr create(const std::function<void()>& lambda) { return std::make_shared<CLASS>(new CLASS(), [lambda](CLASS* obj) { ObjectPool::GetInstance()->release(obj)}); }


//PS:Ҫ��Ӳ�Ա�֤��Ա����ֻ��һ����Ԫ�����,�ó�Ա��������ͨ��Ҳ���ܷ����Ǿ�ֻ������static����ί��,ʵ��̫�鷳��,���ϵ�create����Ӳ�Ա�֤,��Ҫ�Լ��Ծ�


//ObjectPool����Ķ���ʹ��һ�������

#define ACQUIRE_FUNC(__TYPE__)                        \
static std::shared_ptr<__TYPE__> acquire()            \
{                                          \
	__TYPE__ p; \
	std::shared_ptr<__TYPE__> pRet = ObjectPool::GetInstance()->acquire(&p);     \
	if(!(pRet&&pRet->init())){std::cout<<"acquire failed!!"<<std::endl;return nullptr;}							   \
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
//����Ĺ��캯��Ϊprotected,�������ⲿnew����,ֻ��ʹ��ObjectPool::acquire()������
class Object :public std::enable_shared_from_this<Object>
{
	OBJECT_SETUP(Object)
public:

	ACQUIRE_FUNC(Object)

		//��Ԫ����
	template <typename T>
	friend std::shared_ptr<T> ToPtr(T* op);
	template <typename T>
	friend std::shared_ptr<const T> ToCPtr(const T* op);

	//�����ܱ�ObjectPool���ʲ�������
	//friend class ObjectPool;


	//CREATE_FUNC(Object);

	virtual bool init() { return true; }
	//����
	//void retain();
	//�ͷ�
	//void release();



	//��Ԫ
	//friend class AutoreleasePool;

	//virtual ~Object() {}
protected:

	static void ProtectedDelete(const Object* op) {
		delete op;
	}
	//���������ռ�
	using std::enable_shared_from_this<Object>::shared_from_this;




};


GLRT_END
