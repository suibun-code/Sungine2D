#include "MovementSystem.h"

#include <algorithm>

#include "Core.h"
#include "Transform.h"
#include "Movement.h"
#include "Collider.h"

void MovementSystem::Init()
{

}

void MovementSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);
		auto& movement = ECSHandler::Instance()->GetComponent<Movement>(entity);

		movement.velocity += movement.acceleration * deltaTime;

		if (transform.IsDirty() || movement.velocity.x != 0 || movement.velocity.y != 0)
			transform.AddPosition(movement.velocity * deltaTime);

		if (!movement.accelerated)
			continue;

		if (movement.velocity.x != 0 || movement.velocity.y != 0)
		{
			if (movement.velocity.x < 0)
			{
				movement.velocity.x *= .99f;
				movement.acceleration.x *= .99f;
			}
			else if (movement.velocity.x > 0)
			{
				movement.velocity.x *= .99f;
				movement.acceleration.x *= .99f;
			}
			if (movement.velocity.y < 0)
			{
				movement.velocity.y *= .99f;
				movement.acceleration.y *= .99f;
			}
			else if (movement.velocity.y > 0)
			{
				movement.velocity.y *= .99f;
				movement.acceleration.y *= .99f;
			}
		}

		//if (movement.velocity.x < 0.1f)
		//	movement.velocity.x = 0;
	}
}

