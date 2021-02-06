#pragma once

#include "BehaviourScript.h"
#include "SuTexture2D.h"
#include "Types.h"

class Tile : public BehaviourScript
{
private:
	
public:
	Tile();

	virtual void Start() override;
	virtual void Destroy() override;
	virtual void Update(float deltaTime) override;

	virtual bool OnCollision(ECSEntity other) override;
};
