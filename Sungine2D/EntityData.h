#pragma once

#include "BehaviourScript.h"

#include <iostream>

struct EntityData
{
	std::string name;
	std::string tag;
	BehaviourScript* script = nullptr;
};