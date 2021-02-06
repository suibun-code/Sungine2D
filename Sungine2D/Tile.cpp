#include "Tile.h"

#include "ECSHandler.h"
#include "ResourceManager.h"

#include "EntityData.h"
#include "Rendering.h"
#include "Transform.h"

Tile::Tile()
{
	BehaviourScript::Generate();
}

void Tile::Start()
{
	mEntity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Tile";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).script = this;

	ECSHandler::Instance()->AddComponent(mEntity, Transform{ });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite") });
}

void Tile::Destroy()
{
	BehaviourScript::Destroy();
}

void Tile::Update(float deltaTime)
{
}

bool Tile::OnCollision(ECSEntity other)
{
	return false;
}
