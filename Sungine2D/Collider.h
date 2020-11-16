#pragma once

#include "glm/glm/glm.hpp"

struct Collider
{
	bool moveable = false;
	glm::vec2 boundingBox = glm::vec2(1.f);
	glm::vec2 offset = glm::vec2(0.f);
	bool colliding = false;
};