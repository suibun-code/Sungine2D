#include "CollisionSystem.h"

#include "EntityData.h"
#include "Transform.h"
#include "Collider.h"
#include "Rendering.h"
#include "Bullet.h"
#include "Enemy.h"

#include "Core.h"

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
				auto& colliderOther = ECSHandler::Instance()->GetComponent<Collider>(other);

				if (colliderOther.overlapper == true)
					continue;

				auto& transformOther = ECSHandler::Instance()->GetComponent<Transform>(other);
				auto& dataOther = ECSHandler::Instance()->GetComponent<EntityData>(other);

				//X-axis
				bool collisionX = (transform.position.x + collider.offset.x) + collider.boundingBox.x >= (transformOther.position.x + colliderOther.offset.x) &&
					(transformOther.position.x + colliderOther.offset.x) + colliderOther.boundingBox.x >= (transform.position.x + collider.offset.x);

				//Y-axis
				bool collisionY = (transform.position.y + collider.offset.y) + collider.boundingBox.y >= (transformOther.position.y + colliderOther.offset.y) &&
					(transformOther.position.y + colliderOther.offset.y) + colliderOther.boundingBox.y >= (transform.position.y + collider.offset.y);

				//if (ECSHandler::Instance()->HasComponent<Enemy>(entity))
				//	if (ECSHandler::Instance()->HasComponent<Player>(other))
				//	{
				//		continue;
				//	}

				//if (ECSHandler::Instance()->HasComponent<Player>(entity))
				//	if (ECSHandler::Instance()->HasComponent<Enemy>(other))
				//	{
				//		if (collisionX && collisionY)
				//		{
				//			ECSHandler::Instance()->GetComponent<Player>(other).health -= 1;
				//		}
				//		continue;
				//	}

				if (ECSHandler::Instance()->HasComponent<Player>(entity) || ECSHandler::Instance()->HasComponent<Enemy>(entity))
				{
					if (ECSHandler::Instance()->HasComponent<Player>(other))
					{
						if (collisionX && collisionY)
						{
							ECSHandler::Instance()->GetComponent<Player>(other).health -= 1;
						}
						continue;
					}

					if (ECSHandler::Instance()->HasComponent<Enemy>(other) && !ECSHandler::Instance()->HasComponent<Enemy>(entity))
					{
						continue;
					}
				}

				if (collider.overlapper == true)
				{
					if (ECSHandler::Instance()->HasComponent<Bullet>(entity))
					{
						if (collisionX && collisionY)
						{
							if (dataOther.tag == "Player")
								return;

							else if (dataOther.tag == "Enemy")
							{
								auto& enemyOther = ECSHandler::Instance()->GetComponent<Enemy>(other);
								auto& renderOther = ECSHandler::Instance()->GetComponent<Rendering>(other);

								enemyOther.health -= 25;

								if (enemyOther.health == 50)
									renderOther.color = glm::vec3(1.f, 1.f, 0.f);

								if (enemyOther.health == 25)
									renderOther.color = glm::vec3(1.f, 0.f, 0.f);

								if (enemyOther.health <= 0)
								{
									ResourceManager::ClearText(dataOther.name);
									ECSHandler::Instance()->DestroyEntity(other);
								}

								ECSHandler::Instance()->DestroyEntity(entity);
								return;
							}
							else
							{
								ECSHandler::Instance()->DestroyEntity(entity);
								return;
							}
						}

						if (transform.position.x < 0 || transform.position.x > Core::Instance()->GetWindowWidth() || transform.position.y < 0 || transform.position.y > Core::Instance()->GetWindowHeight())
						{
							ECSHandler::Instance()->DestroyEntity(entity);
							return;
						}
					}
					continue;
				}

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
