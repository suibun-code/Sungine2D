#include "PlayerCharacter.h"

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

	ECSHandler::Instance()->AddComponent(mEntity, Transform{ glm::vec2(600.f, 450.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char") });
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
	float rotation = std::atan2(direction.y, direction.x) * 180.f / (float)M_PI + 90;
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

	//Shooting bullets
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
	}
	
	return false;
}
