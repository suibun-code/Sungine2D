#include "Bullet.h"

#include "ResourceManager.h"

#include "Transform.h"
#include "Rendering.h"
#include "Movement.h"
#include "Collider.h"
#include "Character.h"
#include "Core.h"

int Bullet::mBulletCount = 0;

Bullet::Bullet()
{
	BehaviourScript::Generate();
}

Bullet::~Bullet()
{

}

void Bullet::Start()
{
	mTexture = ResourceManager::GetTexture("bullet");

	mEntity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Bullet";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).script = this;

	ECSHandler::Instance()->AddComponent(mEntity, Transform{  });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), mTexture });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 450.f });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true, true });

	mBulletCount++;
}

void Bullet::Destroy()
{
	mBulletCount--;
	BehaviourScript::Destroy();
}

void Bullet::Update(float deltaTime)
{

}

bool Bullet::OnCollision(ECSEntity other)
{
	auto& dataOther = ECSHandler::Instance()->GetComponent<EntityData>(other);
	
	if (dataOther.tag == "Player")
		return false;

	if (ECSHandler::Instance()->GetComponent<Collider>(other).trigger == true)
		return false;
	
	if (dataOther.tag == "Enemy")
	{
		auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);
		auto& movementOther = ECSHandler::Instance()->GetComponent<Movement>(other);

		movementOther.acceleration += movement.velocity * .55f;
		
		ECSHandler::Instance()->DisableComponent<Rendering>(mEntity);
		ECSHandler::Instance()->DisableComponent<Collider>(mEntity);
		ECSHandler::Instance()->DisableComponent<Movement>(mEntity);
		return true;
	}

	if (dataOther.tag == "Wall")
	{
		ECSHandler::Instance()->DestroyEntity(other);

		ECSHandler::Instance()->DisableComponent<Rendering>(mEntity);
		ECSHandler::Instance()->DisableComponent<Collider>(mEntity);
		ECSHandler::Instance()->DisableComponent<Movement>(mEntity);
		return true;
	}

	if (dataOther.tag == "Breakable")
	{
		std::cout << "bullet hit this wall\n";

		ECSHandler::Instance()->GetComponent<EntityData>(other).script->Destroy();
	}

	ECSHandler::Instance()->DisableComponent<Rendering>(mEntity);
	ECSHandler::Instance()->DisableComponent<Collider>(mEntity);
	ECSHandler::Instance()->DisableComponent<Movement>(mEntity);
	return true;
}


