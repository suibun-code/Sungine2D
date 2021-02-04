#pragma once

#include "BehaviourScript.h"

#include "SuTexture2D.h"
#include "EntityData.h"
#include "ECSHandler.h"

class Bullet : public BehaviourScript
{
private:
	

public:
	static int mBulletCount;
	SuTexture2D mTexture;

	Bullet();
	~Bullet();

	void Start() override;
	void Destroy() override;
	void Update(float deltaTime) override;
	
	bool OnCollision(ECSEntity other) override;
};
