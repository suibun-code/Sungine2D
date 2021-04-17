#include "PlayerCharacter.h"

#include "BreakableWall.h"
#include "ResourceManager.h"

#include "Core.h"

PlayerCharacter::PlayerCharacter()
{
	BehaviourScript::Generate();
}

void PlayerCharacter::Start()
{
	mEntity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).name = "Player";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Player";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).script = this;

	ECSHandler::Instance()->AddComponent(mEntity, Transform{ glm::vec2(310.f, 360.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char"), glm::vec4(1.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 950.f, true });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });
	ECSHandler::Instance()->AddComponent(mEntity, Character{ 1000, ResourceManager::AddText("PlayerHP", "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 255, 125, 0, 255 }) });

	auto& character = ECSHandler::Instance()->GetComponent<Character>(mEntity);
	ECSHandler::Instance()->GetComponent<Text>(character.healthText).ChangeText(std::to_string(15));

	//Bullet object pool.
	for (int i = 0; i < 3; i++)
	{
		Bullet* bullet = new Bullet();
		mBulletOP.push(bullet);
	}
	tempBullet = mBulletOP.front();
}

void PlayerCharacter::Update(float deltaTime)
{
	auto& transform = ECSHandler::Instance()->GetComponent<Transform>(mEntity);
	auto& character = ECSHandler::Instance()->GetComponent<Character>(mEntity);
	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);
	
	ECSHandler::Instance()->GetComponent<Text>(character.healthText).ChangeText(std::to_string(character.health));

	if (ECSHandler::Instance()->GetComponent<Transform>(mEntity).IsDirty())
		ECSHandler::Instance()->GetComponent<Transform>(character.healthText).SetPosition(glm::vec2(transform.position.x + 5, transform.position.y - 25));

	movement.velocity = glm::vec2(0.f);

	//Set the rotation of the character to face towards the mouse cursor.
	glm::vec2 direction = glm::normalize(Core::Instance()->GetMousePos() - (transform.position));
	rotation = std::atan2(direction.y, direction.x) * 180.f / (float)M_PI + 90;
	transform.SetRotation(rotation);

	if (Core::Instance()->KeyDown(SDL_SCANCODE_A))
	{
		movement.acceleration.x += -movement.speed * deltaTime;
		transform.dirty = true;
	}
	else if (Core::Instance()->KeyDown(SDL_SCANCODE_D))
	{
		movement.acceleration.x += movement.speed * deltaTime;
		transform.dirty = true;
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_W))
	{
		movement.acceleration.y += -movement.speed * deltaTime;
		transform.dirty = true;
	}
	else if (Core::Instance()->KeyDown(SDL_SCANCODE_S))
	{
		movement.acceleration.y += movement.speed * deltaTime;
		transform.dirty = true;
	}

	//Shooting bullets.
	if (Core::Instance()->GetLMBState())
	{
		Core::Instance()->GetAM()->PlaySound(0);

		tempBullet = mBulletOP.front();
		tempBullet->SetOwner(tempBullet);

		auto& bulletTransform = ECSHandler::Instance()->GetComponent<Transform>(tempBullet->GetEntity());
		auto& bulletRender = ECSHandler::Instance()->GetComponent<Rendering>(tempBullet->GetEntity());
		auto& bulletMovement = ECSHandler::Instance()->GetComponent<Movement>(tempBullet->GetEntity());
		auto& bulletCollider = ECSHandler::Instance()->GetComponent<Collider>(tempBullet->GetEntity());

		bulletTransform = Transform{ glm::vec2(transform.position.x + (transform.size.x * .5f), transform.position.y + (transform.size.y * .2f)), glm::vec2(1.f), 90.f };
		bulletTransform.SetSize(glm::vec2(bulletRender.texture.Width, bulletRender.texture.Height) * bulletTransform.scale);
		bulletCollider.boundingBox = bulletTransform.size;

		glm::vec2 direction = glm::normalize(Core::Instance()->GetMousePos() - bulletTransform.position);
		bulletMovement.velocity = glm::vec2(direction.x * bulletMovement.speed, direction.y * bulletMovement.speed);
		float rotation = std::atan2(direction.y, direction.x) * 180.f / (float)M_PI;
		
		bulletTransform.SetRotation(rotation);

		ECSHandler::Instance()->EnableComponent<Rendering>(tempBullet->GetEntity());
		ECSHandler::Instance()->EnableComponent<Collider>(tempBullet->GetEntity());
		ECSHandler::Instance()->EnableComponent<Movement>(tempBullet->GetEntity());
		
		mBulletOP.pop();
		mBulletOP.push(tempBullet);
	}

	//Spawning breakable wall.
	if (Core::Instance()->GetRMBState())
	{
		Core::Instance()->GetAM()->PlaySound(2);

		BreakableWall* wall = new BreakableWall();

		auto& wallTransform = ECSHandler::Instance()->GetComponent<Transform>(wall->GetEntity());
		auto& wallRender = ECSHandler::Instance()->GetComponent<Rendering>(wall->GetEntity());
		auto& wallCollider = ECSHandler::Instance()->GetComponent<Collider>(wall->GetEntity());

		if (rotation > -45.f && rotation < 45.f) // Up
		{
			wallTransform = Transform{ glm::vec2(transform.position.x - 14.f, transform.position.y - 30.f) };
			wallTransform.SetSize(glm::vec2(wallRender.texture.Width, wallRender.texture.Height) * wallTransform.scale);
		}
		else if (rotation > 45.f && rotation < 135.f) // Right
		{
			wallTransform = Transform{ glm::vec2(transform.position.x + 46.f, transform.position.y - 14.f)};
			wallTransform.SetSize(glm::vec2(wallRender.texture.Height, wallRender.texture.Width) * wallTransform.scale);
		}
		else if (rotation > 135.f && rotation < 225.f) // Down
		{
			wallTransform = Transform{ glm::vec2(transform.position.x - 14.f, transform.position.y + 46.f) };
			wallTransform.SetSize(glm::vec2(wallRender.texture.Width, wallRender.texture.Height) * wallTransform.scale);
		}
		else if (rotation > 225.f && rotation < 270.f || rotation > -90.f && rotation < -45.f) // Left
		{
			wallTransform = Transform{ glm::vec2(transform.position.x - 30.f, transform.position.y - 14.f) };
			wallTransform.SetSize(glm::vec2(wallRender.texture.Height, wallRender.texture.Width) * wallTransform.scale);
		}
		
		wallCollider.boundingBox = wallTransform.size;
	}
}

void PlayerCharacter::Destroy()
{
	BehaviourScript::Destroy();
}

bool PlayerCharacter::OnCollision(ECSEntity other)
{
	auto& tag = ECSHandler::Instance()->GetComponent<EntityData>(other).tag;
	auto& player = ECSHandler::Instance()->GetComponent<Character>(mEntity);
	
	if (tag == "DamageTile")
		player.health -= 1;

	if (tag == "Enemy")
	{
		auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);
		auto& movementOther = ECSHandler::Instance()->GetComponent<Movement>(other);

		if (std::abs(movement.velocity.x + movement.velocity.y) > std::abs(movementOther.velocity.x + movementOther.velocity.y))
		{
			movementOther.acceleration += movement.acceleration * 1.5f;
			movement.acceleration *= 0.f;
		}
		else
		{
			movement.acceleration += movementOther.acceleration * 1.5f;
			movementOther.acceleration *= 0.f;
		}

		Core::Instance()->GetAM()->PlaySound(1);
	}
	
	return false;
}
