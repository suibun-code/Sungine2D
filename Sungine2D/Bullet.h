#pragma once

#include "BehaviourScript.h"

#include "SuTexture2D.h"
#include "EntityData.h"
#include "ECSHandler.h"

class Bullet : public BehaviourScript
{
private:
	ECSEntity mEntity;

public:
	static int mBulletCount;
	static SuTexture2D mTexture;

	Bullet();
	~Bullet();

	void Start() override;
	void Update() override;

	ECSEntity GetEntity();
	static void OnCollision();
};
