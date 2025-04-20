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
			//set����Ҫ�ж��Ƿ����,��ʹ������ʹ��eraseҲ���ᱨ��
			//��������������,��Ȼentity��ǩ���ı���,
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
	//ά��ÿ��ϵͳ��ǩ��,��ÿ��ϵͳ������������
	std::unordered_map<const char*, Signature> mSignatures{};
	//ά��ÿ��ϵͳ��ʵ��
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
};
ECS_END