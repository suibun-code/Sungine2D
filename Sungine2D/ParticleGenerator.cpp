#include "ParticleGenerator.h"

#include "ECSHandler.h"
#include "Movement.h"
#include "Transform.h"
//
//ParticleGenerator::ParticleGenerator(ShaderUtil shader, SuTexture2D texture, unsigned int amount)
//    : shader(shader), texture(texture), amount(amount)
//{
//    this->Init();
//}
//
//void ParticleGenerator::Update(float dt, ECSEntity object, unsigned int newParticles, glm::vec2 offset)
//{
//    // add new particles 
//    for (unsigned int i = 0; i < newParticles; ++i)
//    {
//        int unusedParticle = this->FirstUnusedParticle();
//        this->RespawnParticle(this->particles[unusedParticle], object, offset);
//    }
//    // update all particles
//    for (unsigned int i = 0; i < this->amount; ++i)
//    {
//        Particle& p = this->particles[i];
//        p.Life -= dt; // reduce life
//        if (p.Life > 0.0f)
//        {	// particle is alive, thus update
//            p.Position -= p.Velocity * dt;
//            p.Color.a -= dt * 2.5f;
//        }
//    }
//}
//
//// render all particles
//void ParticleGenerator::Draw()
//{
//    // use additive blending to give it a 'glow' effect
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//    this->shader.Use();
//	
//    for (Particle particle : this->particles)
//    {
//        if (particle.Life > 0.0f)
//        {
//            this->shader.SetVector2f("offset", particle.Position);
//            this->shader.SetVector4f("color", particle.Color);
//            this->texture.Bind();
//            glBindVertexArray(this->VAO);
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//            glBindVertexArray(0);
//        }
//    }
//	
//    // don't forget to reset to default blending mode
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//void ParticleGenerator::Init()
//{
//    // create this->amount default particle instances
//    for (unsigned int i = 0; i < this->amount; ++i)
//        this->particles.push_back(Particle());
//}
//
//// stores the index of the last particle used (for quick access to next dead particle)
//unsigned int lastUsedParticle = 0;
//unsigned int ParticleGenerator::FirstUnusedParticle()
//{
//    // first search from last used particle, this will usually return almost instantly
//    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
//        if (this->particles[i].Life <= 0.0f) {
//            lastUsedParticle = i;
//            return i;
//        }
//    }
//    // otherwise, do a linear search
//    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
//        if (this->particles[i].Life <= 0.0f) {
//            lastUsedParticle = i;
//            return i;
//        }
//    }
//    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
//    lastUsedParticle = 0;
//    return 0;
//}
//
//void ParticleGenerator::RespawnParticle(Particle& particle, ECSEntity object, glm::vec2 offset)
//{
//    float random = ((rand() % 100) - 50) / 10.0f;
//    float rColor = 0.5f + ((rand() % 100) / 100.0f);
//    particle.Position = ECSHandler::Instance()->GetComponent<Transform>(object).position + random + offset;
//    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
//    particle.Life = 5.0f;
//    particle.Velocity = ECSHandler::Instance()->GetComponent<Movement>(object).velocity * 0.1f;
//
//    std::cout << "posx: " << particle.Position.x << "\n";
//    std::cout << "posy: " << particle.Position.y << "\n\n";
//}