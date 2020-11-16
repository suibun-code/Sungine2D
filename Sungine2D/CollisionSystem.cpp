#include "CollisionSystem.h"

#include "Transform.h"
#include "Collider.h"
#include "Rendering.h"

void CollisionSystem::Init()
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);
		auto& collider = ECSHandler::Instance()->GetComponent<Collider>(entity);

		if (ECSHandler::Instance()->HasComponent<Rendering>(entity))
		{
			auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entity);

			collider.boundingBox.x = (float)render.texture.Width;
			collider.boundingBox.y = (float)render.texture.Height;
		}
	}
}

void CollisionSystem::Update()
{
	xDepth = 0.f;
	xHighestDepth = HighestDepth::RIGHT;
	yHighestDepth = HighestDepth::BOTTOM;

	for (auto const& entity : mEntities)
	{
		auto& collider = ECSHandler::Instance()->GetComponent<Collider>(entity);

		if (!collider.moveable)
			continue;

		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);

		for (auto const& other : mEntities)
		{
			if (entity != other)
			{
				auto& transformOther = ECSHandler::Instance()->GetComponent<Transform>(other);
				auto& colliderOther = ECSHandler::Instance()->GetComponent<Collider>(other);

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
					xDepth = (transform.position.x + collider.boundingBox.x) - transformOther.position.x;
					xHighestDepth = HighestDepth::RIGHT;

					collider.colliding = true;
				}

				//LEFT SIDE WITH RIGHT SIDE
				if (transformOther.position.x + colliderOther.boundingBox.x >= transform.position.x && transformOther.position.x <= transform.position.x && collisionY)
				{
					xDepth = (transformOther.position.x + colliderOther.boundingBox.x) - transform.position.x;
					xHighestDepth = HighestDepth::LEFT;

					collider.colliding = true;
				}

				//BOTTOM SIDE WITH TOP SIDE
				if (transform.position.y + collider.boundingBox.y >= transformOther.position.y && transform.position.y <= transformOther.position.y && collisionX)
				{
					yDepth = (transform.position.y + collider.boundingBox.y) - transformOther.position.y;
					yHighestDepth = HighestDepth::TOP;

					collider.colliding = true;
				}

				//TOP SIDE WITH BOTTOM SIDE
				if (transformOther.position.y + colliderOther.boundingBox.y >= transform.position.y && transformOther.position.y <= transform.position.y && collisionX)
				{
					yDepth = (transformOther.position.y + colliderOther.boundingBox.y) - transform.position.y;
					yHighestDepth = HighestDepth::BOTTOM;

					collider.colliding = true;
				}

				//Stupid but this works for now. Pushes back a little farther to make sure the depth removal doesn't end at a spot where collision is still happening.
				xDepth += 0.001f;
				yDepth += 0.001f;

				if (collider.colliding == true)
				{
					if (xDepth < yDepth)
					{
						switch (xHighestDepth)
						{
						case HighestDepth::LEFT:
							//std::cout << "LEFT\n";
							transform.position.x += xDepth;
							break;
						case HighestDepth::RIGHT:
							//std::cout << "RIGHT\n";
							transform.position.x -= xDepth;
							break;
						}
					}
					if (yDepth < xDepth)
					{
						switch (yHighestDepth)
						{
						case HighestDepth::TOP:
							//std::cout << "TOP\n";
							transform.position.y -= yDepth;
							break;
						case HighestDepth::BOTTOM:
							//std::cout << "BOTTOM\n";
							transform.position.y += yDepth;
							break;
						}
					}
					if ((xDepth - yDepth) < 1.0f)
					{
						//if (collider.colliding == true)
						//{
						//	switch (highestdepth)
						//	{
						//	case highestdepth::left:
						//		//std::cout << "left\n";
						//		transform.position.x += xdepth;
						//		break;
						//	case highestdepth::right:
						//		//std::cout << "right\n";
						//		transform.position.x -= xdepth;
						//		break;
						//	case highestdepth::top:
						//		std::cout << "top\n";
						//		transform.position.y -= ydepth;
						//		break;
						//	case highestdepth::bottom:
						//		std::cout << "bottom\n";
						//		transform.position.y += ydepth;
						//		break;
						//	}
						//}
					}
				}

				collider.colliding = false;
			}
		}
	}
}
