#pragma once

#include <vector>

//GLM
#include "BehaviourScript.h"
#include "glm/glm/glm.hpp"

#include "ShaderUtil.h"
#include "SuTexture2D.h"
#include "Types.h"

class ParticleGenerator : public BehaviourScript
{
private:
	std::vector<ECSEntity> particles;
	ECSEntity followEntity = NULL;

	glm::vec2 offset = glm::vec2(0);
	
	unsigned int newParticles = 0;
	unsigned int amount;
	
	unsigned int FirstUnusedParticle();
	void RespawnParticle(ECSEntity particle, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

public:
	ParticleGenerator(unsigned int amount, unsigned int newParticles, ECSEntity followEntity = NULL, glm::vec2 offset = glm::vec2(0));
	
	void Start() override;
	void Update(float deltaTime) override;
	void Destroy() override;

	void FollowEntity(ECSEntity entityToFollow);
	void SetOffset(glm::vec2 offset);

	bool OnRender(ECSEntity entity) override;
};