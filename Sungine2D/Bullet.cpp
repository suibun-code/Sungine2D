#include "Bullet.h"

#include "ResourceManager.h"

#include "Transform.h"
#include "Rendering.h"
#include "Movement.h"
#include "Collider.h"
#include "Character.h"

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
	if (ECSHandler::Instance()->GetComponent<EntityData>(other).tag == "Player")
		return false;

	if (ECSHandler::Instance()->GetComponent<Collider>(other).trigger == true)
		return false;
	
	if (ECSHandler::Instance()->GetComponent<EntityData>(other).tag == "Enemy")
	{
		auto& enemyOther = ECSHandler::Instance()->GetComponent<Character>(other);
		auto& movementOther = ECSHandler::Instance()->GetComponent<Movement>(other);

		movementOther.acceleration += glm::vec2(0, -100.f);
		
		//enemyOther.health -= 25;
		ECSHandler::Instance()->DisableComponent<Rendering>(mEntity);
		ECSHandler::Instance()->DisableComponent<Collider>(mEntity);
		ECSHandler::Instance()->DisableComponent<Movement>(mEntity);
		return true;
	}

	ECSHandler::Instance()->DisableComponent<Rendering>(mEntity);
	ECSHandler::Instance()->DisableComponent<Collider>(mEntity);
	ECSHandler::Instance()->DisableComponent<Movement>(mEntity);
	return true;
}


