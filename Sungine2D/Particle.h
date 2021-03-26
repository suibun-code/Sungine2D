#pragma once

//GLM
#include "glm/glm/glm.hpp"

struct Particle
{
	glm::vec2 position = glm::vec2(0.f);
	glm::vec2 velocity = glm::vec2(0.f);
	glm::vec4 color = glm::vec4(1.f);
	float life = 0.f;
};
