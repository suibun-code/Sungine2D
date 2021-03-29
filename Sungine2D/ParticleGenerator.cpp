#include "ParticleGenerator.h"

//Components
#include "EntityData.h"
#include "Rendering.h"
#include "Movement.h"
#include "Particle.h"
#include "Transform.h"

#include "ECSHandler.h"
#include "ResourceManager.h"

unsigned int lastUsedParticle = 0;

ParticleGenerator::ParticleGenerator(unsigned int amount, unsigned int newParticles, ECSEntity followEntity, glm::vec2 offset) : amount(amount), newParticles(newParticles), followEntity(followEntity), offset(offset)
{
    BehaviourScript::Generate();
    Start();
}

void ParticleGenerator::Start()
{
    mEntity = ECSHandler::Instance()->CreateEntity();
	
    ECSHandler::Instance()->GetComponent<EntityData>(mEntity).script = this;
    ECSHandler::Instance()->GetComponent<EntityData>(mEntity).name = "FollowParticles";
    ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "ParticleGenerator";
	
    ResourceManager::LoadTexture("res/img/particle.png", true, "particle");

    // create amount default particle instances
    for (unsigned int i = 0; i < amount; ++i)
    {
        ECSEntity particle = ECSHandler::Instance()->CreateEntity();
        ECSHandler::Instance()->GetComponent<EntityData>(particle).script = this;
        ECSHandler::Instance()->GetComponent<EntityData>(particle).name = "particle";
        ECSHandler::Instance()->GetComponent<EntityData>(particle).tag = "Particle";
        ECSHandler::Instance()->AddComponent(particle, Transform{ });
        ECSHandler::Instance()->AddComponent(particle, Rendering{ ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle") });
        ECSHandler::Instance()->AddComponent(particle, Particle{});

        particles.push_back(particle);
    }
}

void ParticleGenerator::Update(float deltaTime)
{
    //Add new particles.
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = FirstUnusedParticle();
        RespawnParticle(particles[unusedParticle], offset);
    }

    //Update all particles.
    for (unsigned int i = 0; i < amount; ++i)
    {
        ECSEntity& particle = particles[i];

        auto& p = ECSHandler::Instance()->GetComponent<Particle>(particle);

        p.life -= deltaTime; // reduce life
        if (p.life > 0.0f)
        {	// particle is alive, thus update
            p.position -= p.velocity * deltaTime;
            p.color.a -= deltaTime * 1.75f;
        }
    }
}

void ParticleGenerator::Destroy()
{
    BehaviourScript::Destroy();
}

void ParticleGenerator::FollowEntity(ECSEntity entityToFollow)
{
    followEntity = entityToFollow;
}

void ParticleGenerator::SetOffset(glm::vec2 offset)
{
    this->offset = offset;
}

//Stores the index of the last particle used (for quick access to next dead particle).
unsigned int ParticleGenerator::FirstUnusedParticle()
{
    //First search from last used particle, this will usually return almost instantly.
    for (unsigned int i = lastUsedParticle; i < amount; ++i) 
    {
        auto& particleComponent = ECSHandler::Instance()->GetComponent<Particle>(particles[i]);
    	
        if (particleComponent.life <= 0.0f) 
        {
            lastUsedParticle = i;
            return i;
        }
    }
	
    //Otherwise, do a linear search.
    for (unsigned int i = 0; i < lastUsedParticle; ++i) 
    {
        auto& particleComponent = ECSHandler::Instance()->GetComponent<Particle>(particles[i]);
    	
        if (particleComponent.life <= 0.0f) 
        {
            lastUsedParticle = i;
            return i;
        }
    }
	
    //All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved).
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::RespawnParticle(ECSEntity particle, glm::vec2 offset)
{
    auto& particleComponent = ECSHandler::Instance()->GetComponent<Particle>(particle);
	
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
	
    particleComponent.position = ECSHandler::Instance()->GetComponent<Transform>(followEntity).position + random + offset;
    particleComponent.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particleComponent.life = 0.75f;
    particleComponent.velocity = ECSHandler::Instance()->GetComponent<Movement>(followEntity).velocity * 0.1f;
}

bool ParticleGenerator::OnRender(ECSEntity entity)
{
    auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entity);
    auto& particle = ECSHandler::Instance()->GetComponent<Particle>(entity);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    render.shaderUtil.Use();

    if (particle.life < 0.0f)
    {
        return false;
    }

    //Render.
    render.shaderUtil.SetVector2f("offset", particle.position);
    render.shaderUtil.SetVector4f("color", particle.color);

    return true;
}
