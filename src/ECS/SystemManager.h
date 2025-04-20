#pragma once

#include "System.h"
#include "Types.h"
#include <cassert>
#include <memory>
#include <unordered_map>

ECS_BEGIN
class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		auto system = std::make_shared<T>();
		mSystems.insert({typeName, system});
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

		mSignatures.insert({typeName, signature});
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;


			system->mEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];
			//set不需要判断是否存在,即使不存在使用erase也不会报错
			//他这个命名好奇怪,既然entity的签名改变了,
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}

private:
	//维护每个系统的签名,即每个系统所需的组件类型
	std::unordered_map<const char*, Signature> mSignatures{};
	//维护每个系统的实例
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
};
ECS_END