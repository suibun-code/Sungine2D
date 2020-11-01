#pragma once

#include <set>

#include "Types.h"

class ECSSystem
{
public:
	std::set<ECSEntity> mEntities;
};