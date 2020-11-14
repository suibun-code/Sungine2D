#include "CollisionSystem.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"

void CollisionSystem::Init()
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<TransformComponent>(entity);
		auto& collider = ECSHandler::Instance()->GetComponent<ColliderComponent>(entity);

		if (ECSHandler::Instance()->HasComponent<RenderComponent>(entity))
		{
			auto& render = ECSHandler::Instance()->GetComponent<RenderComponent>(entity);

			collider.boundingBox.x = (float)render.texture.Width;
			collider.boundingBox.y = (float)render.texture.Height;
		}
	}
}

void CollisionSystem::Update()
{
	depth = 0.f;
	highestDepth = HighestDepth::RIGHT;

	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<TransformComponent>(entity);
		auto& collider = ECSHandler::Instance()->GetComponent<ColliderComponent>(entity);
		MovementComponent movement;

		if (collider.moveable)
			movement = ECSHandler::Instance()->GetComponent<MovementComponent>(entity);

		for (auto const& other : mEntities)
		{
			if (entity != other)
			{
				auto& transformOther = ECSHandler::Instance()->GetComponent<TransformComponent>(other);
				auto& colliderOther = ECSHandler::Instance()->GetComponent<ColliderComponent>(other);

				if (collider.moveable == true)
				{
					//X-axis
					bool collisionX = (transform.position.x + collider.offset.x) + collider.boundingBox.x >= (transformOther.position.x + colliderOther.offset.x) &&
						(transformOther.position.x + colliderOther.offset.x) + colliderOther.boundingBox.x >= (transform.position.x + collider.offset.x);

					//Y-axis
					bool collisionY = (transform.position.y + collider.offset.y) + collider.boundingBox.y >= (transformOther.position.y + colliderOther.offset.y) &&
						(transformOther.position.y + colliderOther.offset.y) + colliderOther.boundingBox.y >= (transform.position.y + collider.offset.y);

					//No collision is detected. Continue to next entity before doing advanced checks as there is no need.
					if (!(collisionX && collisionY))
						continue;

					//RIGHT SIDE WITH LEFT SIDE
					if (transform.position.x + collider.boundingBox.x >= transformOther.position.x && transform.position.x <= transformOther.position.x && collisionY)
					{
							depth = (transform.position.x + collider.boundingBox.x) - transformOther.position.x;
							highestDepth = HighestDepth::RIGHT;

							collider.colliding = true;
					}

					//LEFT SIDE WITH RIGHT SIDE
					if (transformOther.position.x + colliderOther.boundingBox.x >= transform.position.x && transformOther.position.x <= transform.position.x && collisionY)
					{
							if (depth != 0)
							{
								if (std::abs(depth) > std::abs((transformOther.position.x + colliderOther.boundingBox.x) - transform.position.x))
								{
									depth = (transformOther.position.x + colliderOther.boundingBox.x) - transform.position.x;
									highestDepth = HighestDepth::LEFT;
								}
							}
							else
							{
								depth = (transformOther.position.x + colliderOther.boundingBox.x) - transform.position.x;
								highestDepth = HighestDepth::LEFT;
							}

							collider.colliding = true;
					}

					//BOTTOM SIDE WITH TOP SIDE
					if (transform.position.y + collider.boundingBox.y >= transformOther.position.y && transform.position.y <= transformOther.position.y && collisionX)
					{
							if (depth != 0)
							{
								if (std::abs(depth) > std::abs((transform.position.y + collider.boundingBox.y) - transformOther.position.y))
								{
									depth = (transform.position.y + collider.boundingBox.y) - transformOther.position.y;
									highestDepth = HighestDepth::TOP;
								}
							}
							else
							{
								depth = (transform.position.y + collider.boundingBox.y) - transformOther.position.y;
								highestDepth = HighestDepth::TOP;
							}

							collider.colliding = true;
					}

					//TOP SIDE WITH BOTTOM SIDE
					if (transformOther.position.y + colliderOther.boundingBox.y >= transform.position.y && transformOther.position.y <= transform.position.y && collisionX)
					{
							if (depth != 0)
							{
								if (std::abs(depth) > std::abs((transformOther.position.y + colliderOther.boundingBox.y) - transform.position.y))
								{
									depth = (transformOther.position.y + colliderOther.boundingBox.y) - transform.position.y;
									highestDepth = HighestDepth::BOTTOM;
								}
							}
							else
							{
								depth = (transformOther.position.y + colliderOther.boundingBox.y) - transform.position.y;
								highestDepth = HighestDepth::BOTTOM;
							}

							collider.colliding = true;
					}

					//Stupid but this works for now. Margin of error to prevent extra collision after one is over.
					depth += 0.01f;

					if (collider.colliding == true)
					{
						switch (highestDepth)
						{
						case HighestDepth::LEFT:
							//std::cout << "LEFT\n";
							transform.position.x += depth;
							break;
						case HighestDepth::RIGHT:
							//std::cout << "RIGHT\n";
							transform.position.x -= depth;
							break;
						case HighestDepth::TOP:
							//std::cout << "TOP\n";
							transform.position.y -= depth;
							break;
						case HighestDepth::BOTTOM:
							//std::cout << "BOTTOM\n";
							transform.position.y += depth;
							break;
						}
					}

					collider.colliding = false;
				}
			}
		}
	}
}
