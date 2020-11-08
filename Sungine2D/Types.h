#pragma once

#include <cstdint>
#include <bitset>

using ECSEntity = std::uint32_t;
using ECSComponentType = std::uint8_t;

const ECSEntity MAX_ENTITIES = 3000;
const ECSComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;