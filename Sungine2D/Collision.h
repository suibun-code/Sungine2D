#pragma once

#include "Entity.h"

class Collision
{
private:

public:
	//AABB collision.
	static bool CheckCollision(Entity& e1, Entity& e2);
};

