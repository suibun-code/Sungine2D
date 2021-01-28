#include "PlayerCharacter.h"

#include "ResourceManager.h"

#include "Transform.h"
#include "Rendering.h"
#include "Movement.h"
#include "Collider.h"
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

	ECSHandler::Instance()->AddComponent(mEntity, Transform{ glm::vec2(1200.f, 600.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char") });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });

	//for (int i = 0; i < 20; i++)
	//{
	//	Bullet* bullet = new Bullet();
	//	mBulletOP.push_back(bullet);
	//}

	std::cout << "Made player\n";
}

void PlayerCharacter::Destroy()
{
}

void PlayerCharacter::Update(float deltaTime)
{
	auto& transform = ECSHandler::Instance()->GetComponent<Transform>(mEntity);
	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);

	movement.velocity = glm::vec2(0.f);

	if (Core::Instance()->KeyDown(SDL_SCANCODE_A))
	{
		movement.velocity.x = -movement.speed * deltaTime;
		transform.dirty = true;
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_D))
	{
		movement.velocity.x = movement.speed * deltaTime;
		transform.dirty = true;
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_W))
	{
		movement.velocity.y = -movement.speed * deltaTime;
		transform.dirty = true;
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_S))
	{
		movement.velocity.y = movement.speed * deltaTime;
		transform.dirty = true;
	}

	if (Core::Instance()->GetLMBState())
	{
		Core::Instance()->GetAM()->PlaySound(0);

		Bullet* testbullet = new Bullet();
		testbullet->SetParent(testbullet);

		auto& bulletTransform = ECSHandler::Instance()->GetComponent<Transform>(testbullet->GetEntity());
		auto& bulletRender = ECSHandler::Instance()->GetComponent<Rendering>(testbullet->GetEntity());
		auto& bulletMovement = ECSHandler::Instance()->GetComponent<Movement>(testbullet->GetEntity());
		auto& bulletCollider = ECSHandler::Instance()->GetComponent<Collider>(testbullet->GetEntity());

		bulletTransform = Transform{ glm::vec2(transform.position.x + (transform.size.x * .5f), transform.position.y + (transform.size.y * .2f)), glm::vec2(1.f), 90.f };

		glm::vec2 mousePos = glm::vec2(Core::Instance()->GetMousePosX(), Core::Instance()->GetMousePosY());
		bulletTransform.SetSize(glm::vec2(bulletRender.texture.Width, bulletRender.texture.Height) * bulletTransform.scale);
		bulletCollider.boundingBox = bulletTransform.size;

		float vectorToTarget = glm::distance(bulletTransform.position, mousePos);

		glm::vec2 direction = glm::normalize(mousePos - bulletTransform.position);
		bulletMovement.velocity = glm::vec2(direction.x * bulletMovement.speed, direction.y * bulletMovement.speed);

		float rotation = std::atan2(direction.y, direction.x) * 180.f / (float)M_PI;
		bulletTransform.SetRotation(rotation);
	}

}

bool PlayerCharacter::OnCollision(ECSEntity other)
{
	return false;
}
