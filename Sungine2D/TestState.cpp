#include "TestState.h"

//GLEW
#include "glew.h"

//Misc
#include "Core.h"
#include "ResourceManager.h"
#include "Level.h"
#include "PlayerCharacter.h"
#include "ParticleGenerator.h"

//States
#include "EnemyCharacter.h"
#include "MainMenu.h"

void TestState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.2f, .2f, .2f, 1.f);

	Core::Instance()->MoveView(glm::vec3(0.f, -24.f, 0.f));

	Core::Instance()->GetAM()->LoadSound("res/audio/effect/lasershoot2.wav");
	Core::Instance()->GetAM()->SetSoundsVolume(0, 10);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/musikkperautomatikk.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	shader = ResourceManager::GetShader("sprite");
	ResourceManager::LoadTexture("res/img/character.png", true, "char");
	ResourceManager::LoadTexture("res/img/bullet3.png", true, "bullet");

	//Loading the level.
	Level levelTest;
	levelTest.Load("res/levels/saved.txt");

	//Spawn locations for the player.
	glm::vec2 spawnLocations[5];
	spawnLocations[0] = glm::vec2(500.f, 250.f);
	spawnLocations[1] = glm::vec2(1000.f, 50.f);
	spawnLocations[2] = glm::vec2(300.f, 65.f);
	spawnLocations[3] = glm::vec2(550.f, 55.f);
	spawnLocations[4] = glm::vec2(600.f, 45.f);

	//Particles for the player.
	ParticleGenerator* particles;
	particles = new ParticleGenerator(100, 1, glm::vec4(.0f, .0f, 1.f, 1.f), glm::vec2(10.f, -10.f));
	particles->SetOwner(particles);
	
	//Particles for the enemy.
	ParticleGenerator* particlesEnemy;
	particlesEnemy = new ParticleGenerator(100, 1, glm::vec4(1.f, .0f, .0f, 1.f), glm::vec2(10.f, -10.f));
	particles->SetOwner(particlesEnemy);

	//Player.
	PlayerCharacter* player;
	player = new PlayerCharacter();
	player->SetOwner(player);

	//Tell the particles to follow the player.
	particles->FollowEntity(player->GetEntity());

	//Spawn enemies at pre-determined locations from the spawnLocations array.
	for (int i = 0; i < 1; i++)
	{
		EnemyCharacter* enemy = new EnemyCharacter();
		enemy->SetOwner(enemy);
		ECSHandler::Instance()->GetComponent<Transform>(enemy->GetEntity()).position = spawnLocations[i];
		ECSHandler::Instance()->AddComponent(enemy->GetEntity(), Follow{ player->GetEntity() });

		//Tell the particles to follow the enemy.
		particlesEnemy->FollowEntity(enemy->GetEntity());
	}

	Core::Instance()->GetSystem<TextSystem>()->Init();
	Core::Instance()->GetSystem<MovementSystem>()->Init();
	Core::Instance()->GetSystem<CollisionSystem>()->Init();
	Core::Instance()->GetSystem<RenderSystem>()->Init();
	Core::Instance()->GetSystem<FollowSystem>()->Init();

	State::Enter();
}

void TestState::HandleStateEvents(SDL_Event* event)
{

}

void TestState::Update(float deltaTime)
{
	Core::Instance()->GetSystem<TextSystem>()->Update();
	Core::Instance()->GetSystem<MovementSystem>()->Update(deltaTime);
	Core::Instance()->GetSystem<CollisionSystem>()->Update();
	Core::Instance()->GetSystem<FollowSystem>()->Update(deltaTime);
	
	State::Update(deltaTime);
}

void TestState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw renderable entities.
	Core::Instance()->GetSystem<RenderSystem>()->Draw();

	State::Render();
}

void TestState::LateUpdate(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu());
		return;
	}
}

void TestState::Exit()
{
	State::Exit();

	Core::Instance()->MoveView(glm::vec3(0.f, 24.f, 0.f));

	std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";
	std::cout << "RenderSystem entity count: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";
}
