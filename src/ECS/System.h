#pragma once

#include "Types.h"
#include <set>

ECS_BEGIN
class System
{
public:
	std::set<Entity> mEntities;
};
ECS_END
