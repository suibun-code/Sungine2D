#include "PlayerCharacter.h"

#include "ResourceManager.h"

#include "Transform.h"
#include "Rendering.h"
#include "Movement.h"
#include "Collider.h"
#include "Character.h"
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

	ECSHandler::Instance()->AddComponent(mEntity, Transform{ glm::vec2(600.f, 500.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char") });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });
	ECSHandler::Instance()->AddComponent(mEntity, Character{ });

	//Bullet object pool.
	for (int i = 0; i < 3; i++)
	{
		Bullet* bullet = new Bullet();
		mBulletOP.push(bullet);
	}

	testbullet = mBulletOP.front();
}

void PlayerCharacter::Destroy()
{
	BehaviourScript::Destroy();
}

void PlayerCharacter::Update(float deltaTime)
{
	auto& transform = ECSHandler::Instance()->GetComponent<Transform>(mEntity);
	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);

	movement.velocity = glm::vec2(0.f);

	//Set the rotation of the character to face towards the mouse cursor.
	glm::vec2 direction = glm::normalize(Core::Instance()->GetMousePos() - (transform.position));
	float rotation = std::atan2(direction.y, direction.x) * 180.f / (float)M_PI + 90;
	transform.SetRotation(rotation);

	if (Core::Instance()->KeyDown(SDL_SCANCODE_A))
	{
		movement.velocity.x = -movement.speed * deltaTime;
		transform.dirty = true;
	}
	else if (Core::Instance()->KeyDown(SDL_SCANCODE_D))

	{
		movement.velocity.x = movement.speed * deltaTime;
		transform.dirty = true;
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_W))
	{
		movement.velocity.y = -movement.speed * deltaTime;
		transform.dirty = true;
	}
	else if (Core::Instance()->KeyDown(SDL_SCANCODE_S))
	{
		movement.velocity.y = movement.speed * deltaTime;
		transform.dirty = true;
	}

	//if (ECSHandler::Instance()->GetComponent<Collider>(testbullet->GetEntity()).colliding == true)
	//{
	//	if (ECSHandler::Instance()->GetComponent<EntityData>(ECSHandler::Instance()->GetComponent<Collider>(testbullet->GetEntity()).other).tag != "Player")
	//	{
	//		ECSHandler::Instance()->DisableComponent<Rendering>(testbullet->GetEntity());
	//		mBulletOP.pop();
	//		mBulletOP.push(testbullet);
	//	}

	//	//auto& bulletTransform = ECSHandler::Instance()->GetComponent<Transform>(mBulletOP.front()->GetEntity());
	//	//bulletTransform.SetPosition(glm::vec2(0, 0));
	//}

	if (Core::Instance()->GetLMBState())
	{
		Core::Instance()->GetAM()->PlaySound(0);

		testbullet = mBulletOP.front();
		testbullet->SetParent(testbullet);

		auto& bulletTransform = ECSHandler::Instance()->GetComponent<Transform>(testbullet->GetEntity());
		auto& bulletRender = ECSHandler::Instance()->GetComponent<Rendering>(testbullet->GetEntity());
		auto& bulletMovement = ECSHandler::Instance()->GetComponent<Movement>(testbullet->GetEntity());
		auto& bulletCollider = ECSHandler::Instance()->GetComponent<Collider>(testbullet->GetEntity());

		bulletTransform = Transform{ glm::vec2(transform.position.x + (transform.size.x * .5f), transform.position.y + (transform.size.y * .2f)), glm::vec2(1.f), 90.f };

		bulletTransform.SetSize(glm::vec2(bulletRender.texture.Width, bulletRender.texture.Height) * bulletTransform.scale);
		bulletCollider.boundingBox = bulletTransform.size;

		glm::vec2 direction = glm::normalize(Core::Instance()->GetMousePos() - bulletTransform.position);
		bulletMovement.velocity = glm::vec2(direction.x * bulletMovement.speed, direction.y * bulletMovement.speed);
		float rotation = std::atan2(direction.y, direction.x) * 180.f / (float)M_PI;

		bulletTransform.SetRotation(rotation);

		ECSHandler::Instance()->EnableComponent<Rendering>(testbullet->GetEntity());
		ECSHandler::Instance()->EnableComponent<Collider>(testbullet->GetEntity());
		
		mBulletOP.pop();
		mBulletOP.push(testbullet);
	}
}

bool PlayerCharacter::OnCollision(ECSEntity other)
{
	return false;
}
