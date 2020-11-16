#pragma once

//GLM
#include "glm/glm/glm.hpp"

struct Transform
{
	float scale = 1.f;
	float rotation = 0.f;
	glm::vec2 position = glm::vec2(0.f, 0.f);
	glm::vec2 size = glm::vec2(1.f, 1.f);
};
