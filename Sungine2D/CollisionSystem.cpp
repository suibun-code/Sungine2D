#include "CollisionSystem.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "RenderComponent.h"

void CollisionSystem::Init()
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<TransformComponent>(entity);
		auto& collider = ECSHandler::Instance()->GetComponent<ColliderComponent>(entity);

		if (ECSHandler::Instance()->HasComponent<RenderComponent>(entity))
		{
			auto& render = ECSHandler::Instance()->GetComponent<RenderComponent>(entity);

			collider.boundingBox.x = render.texture.Width;
			collider.boundingBox.y = render.texture.Height;
		}
	}
}

void CollisionSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<TransformComponent>(entity);
		auto& collider = ECSHandler::Instance()->GetComponent<ColliderComponent>(entity);

		for (auto const& other : mEntities)
		{
			auto& transformOther = ECSHandler::Instance()->GetComponent<TransformComponent>(other);
			auto& colliderOther = ECSHandler::Instance()->GetComponent<ColliderComponent>(other);

			if (entity != other)
			{
				//X-axis
				bool collisionX = (transform.position.x + collider.offset.x) + collider.boundingBox.x >= (transformOther.position.x + colliderOther.offset.x) &&
					(transformOther.position.x + colliderOther.offset.x) + colliderOther.boundingBox.x >= (transform.position.x + collider.offset.x);

				//Y-axis
				bool collisionY = (transform.position.y + collider.offset.y) + collider.boundingBox.y >= (transformOther.position.y + colliderOther.offset.y) &&
					(transformOther.position.y + colliderOther.offset.y) + colliderOther.boundingBox.y >= (transform.position.y + collider.offset.y);

				//Collides only on both axes.
				if (collisionX && collisionY)
				{
					std::cout << "collided\n";
				}
			}
		}
	}
}
