#pragma once

//GLM
#include "glm/glm/glm.hpp"

struct Collider
{
	bool moveable = false;
	bool overlapper = false;
	glm::vec2 boundingBox = glm::vec2(1.f);
	glm::vec2 offset = glm::vec2(0.f);
	bool colliding = false;
};