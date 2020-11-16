#include "MovementSystem.h"

#include <iostream>

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

		movement.velocity = glm::vec2(0.f);

		if (Core::Instance()->KeyDown(SDL_SCANCODE_A))
		{
			movement.velocity.x -= movement.speed * deltaTime;

			//transform.position.x -= movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_D))
		{
			movement.velocity.x += movement.speed * deltaTime;

			//transform.position.x += movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_W))
		{
			movement.velocity.y -= movement.speed * deltaTime;

			//transform.position.y -= movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_S))
		{
			movement.velocity.y += movement.speed * deltaTime;

			//transform.position.y += movement.speed * deltaTime;
		}

		transform.position += movement.velocity;
	}
}

