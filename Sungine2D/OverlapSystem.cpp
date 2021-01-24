#include "OverlapSystem.h"

#include "Transform.h"
#include "Collider.h"
#include "Bullet.h"

void OverlapSystem::Init()
{
}

void OverlapSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);
		auto& collider = ECSHandler::Instance()->GetComponent<Collider>(entity);

		for (auto const& other : mEntities)
		{
			auto& transformOther = ECSHandler::Instance()->GetComponent<Transform>(other);
			auto& colliderOther = ECSHandler::Instance()->GetComponent<Collider>(other);

			//X-axis
			bool collisionX = (transform.position.x + collider.offset.x) + collider.boundingBox.x >= (transformOther.position.x + colliderOther.offset.x) &&
				(transformOther.position.x + colliderOther.offset.x) + colliderOther.boundingBox.x >= (transform.position.x + collider.offset.x);

			//Y-axis
			bool collisionY = (transform.position.y + collider.offset.y) + collider.boundingBox.y >= (transformOther.position.y + colliderOther.offset.y) &&
				(transformOther.position.y + colliderOther.offset.y) + colliderOther.boundingBox.y >= (transform.position.y + collider.offset.y);

			if (ECSHandler::Instance()->HasComponent<Bullet>(entity))
			{
				if (collisionX && collisionY)
				{
					//std::cout << "bullet hit something\n";
				}
			}
		}
	}
}
