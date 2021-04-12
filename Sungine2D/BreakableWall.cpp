#include "BreakableWall.h"

#include "ECSHandler.h"
#include "ResourceManager.h"

//Components
#include "Collider.h"
#include "EntityData.h"
#include "Rendering.h"
#include "Transform.h"

BreakableWall::BreakableWall()
{
	BehaviourScript::Generate();
}

BreakableWall::~BreakableWall()
{
}

void BreakableWall::Start()
{
	mTexture = ResourceManager::GetTexture("wall_breakable");

	mEntity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Breakable";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).script = this;

	ECSHandler::Instance()->AddComponent(mEntity, Transform{  });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), mTexture });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ });
}

void BreakableWall::Destroy()
{
	BehaviourScript::Destroy();
}

void BreakableWall::Update(float deltaTime)
{
}

bool BreakableWall::OnCollision(ECSEntity other)
{
	auto& dataOther = ECSHandler::Instance()->GetComponent<EntityData>(other);
	
	if (dataOther.tag == "Bullet")
	{
		std::cout << "bullet hit this wall\n";
		
		Destroy();
		return true;
	}
	
	return false;
}
