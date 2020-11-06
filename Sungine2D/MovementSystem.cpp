#include "MovementSystem.h"

#include <iostream>

#include "Core.h"
#include "TransformComponent.h"

void MovementSystem::Init()
{
}

void MovementSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<TransformComponent>(entity);

		transform.position.x += 1;

		if (Core::Instance()->KeyDown(SDL_SCANCODE_R))
		{
			std::cout << "WONDABA!\n";
		}
	}
}

