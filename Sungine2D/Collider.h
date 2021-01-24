#pragma once

#include <functional>
#include "Bullet.h"

//GLM
#include "glm/glm/glm.hpp"

struct Collider
{
	bool moveable = false;
	bool overlapper = false;
	glm::vec2 boundingBox = glm::vec2(1.f);
	glm::vec2 offset = glm::vec2(0.f);
	bool colliding = false;

	Collider(bool moveable = false, bool overlapper = false, glm::vec2 boundingBox = glm::vec2(1.f), glm::vec2 offset = glm::vec2(0.f), bool colliding = false) : moveable(moveable), overlapper(overlapper), boundingBox(boundingBox), offset(offset), colliding(colliding)
	{
		
	}

	//std::function<void()> OnCollision = nullptr;

	void (*OnCollision)() = nullptr;

	void Call()
	{
		if (OnCollision != nullptr)
			OnCollision();
	}
};