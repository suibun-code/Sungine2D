#pragma once

struct Movement
{
	float speed = 65000.f;
	bool accelerated = false;
	glm::vec2 velocity = glm::vec2(0.f);
	glm::vec2 acceleration = glm::vec2(0.f);
};