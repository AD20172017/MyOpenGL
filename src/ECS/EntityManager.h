#pragma once

#include "Types.h"
#include <array>
#include <cassert>
#include <queue>

ECS_BEGIN
class EntityManager
{
public:
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			mAvailableEntities.push(entity);
		}
	}

	Entity CreateEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		return id;
	}

	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity].reset();
		mAvailableEntities.push(entity);
		--mLivingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity] = signature;
	}

	Signature GetSignature(Entity entity)
	{
		//为假就抛出错误
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return mSignatures[entity];
	}

private:
	//管理可以分发的实体(id),即保证实体的唯一性
	std::queue<Entity> mAvailableEntities{};
	//每个实例的签名,通过mSignatures[Entity]获取
	std::array<Signature, MAX_ENTITIES> mSignatures{};
	uint32_t mLivingEntityCount{};
};
ECS_END