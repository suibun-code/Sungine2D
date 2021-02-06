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

		if (movement.acceleration.x != 0 || movement.acceleration.y != 0)
		{
			if (movement.acceleration.x < 0)
				movement.acceleration.x += 150.f;

			else if (movement.acceleration.x > 0)
				movement.acceleration.x -= 150.f;

			if (movement.acceleration.y < 0)
				movement.acceleration.y += 150.f;
									  
			else if (movement.acceleration.y > 0)
				movement.acceleration.y -= 150.f;
		}
	}
}

