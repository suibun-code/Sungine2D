#include "TestState.h"

//GLEW
#include "glew.h"

//Misc
#include "EnemyCharacter.h"
#include "Core.h"
#include "ResourceManager.h"
#include "Level.h"
#include "PlayerCharacter.h"
#include "ParticleGenerator.h"

//States
#include "PauseState.h"
#include "WinState.h"

PlayerCharacter* player;
EnemyCharacter* enemy;

void TestState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.2f, .2f, .2f, 1.f);

	Core::Instance()->MoveView(glm::vec3(0.f, -24.f, 0.f));

	Core::Instance()->GetAM()->LoadSound("res/audio/effect/lasershoot2.wav");
	Core::Instance()->GetAM()->LoadSound("res/audio/effect/hurt.wav");
	Core::Instance()->GetAM()->LoadSound("res/audio/effect/placeWall.wav");
	Core::Instance()->GetAM()->LoadSound("res/audio/effect/destroyWall.wav");
	Core::Instance()->GetAM()->LoadSound("res/audio/effect/hitAnything.wav");

	Core::Instance()->GetAM()->SetSoundsVolume(0, 10);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/cpumood.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	shader = ResourceManager::GetShader("sprite");
	ResourceManager::LoadTexture("res/img/character.png", true, "char");
	ResourceManager::LoadTexture("res/img/bullet3.png", true, "bullet");
	ResourceManager::LoadTexture("res/img/wall_breakable.png", true, "wall_breakable");

	//Loading the level.
	Level levelTest;
	levelTest.Load("res/levels/saved.txt");

	//Particles for the player.
	ParticleGenerator* particles;
	particles = new ParticleGenerator(100, 1, glm::vec4(.0f, .0f, 1.f, 1.f), glm::vec2(10.f, -10.f));
	particles->SetOwner(particles);

	//Particles for the enemy.
	ParticleGenerator* particlesEnemy;
	particlesEnemy = new ParticleGenerator(100, 1, glm::vec4(1.f, .0f, .0f, 1.f), glm::vec2(10.f, -10.f));
	particles->SetOwner(particlesEnemy);

	//Player.
	player = new PlayerCharacter();
	player->SetOwner(player);

	//Tell the particles to follow the player.
	particles->FollowEntity(player->GetEntity());

	enemy = new EnemyCharacter();
	enemy->SetOwner(enemy);
	ECSHandler::Instance()->GetComponent<Transform>(enemy->GetEntity()).position = glm::vec2(970.f, 360.f);
	ECSHandler::Instance()->AddComponent(enemy->GetEntity(), Follow{ player->GetEntity() });

	//Tell the particles to follow the enemy.
	particlesEnemy->FollowEntity(enemy->GetEntity());

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

	if (ECSHandler::Instance()->GetComponent<Character>(player->GetEntity()).health <= 0)
	{
		Core::Instance()->GetFSM()->ChangeState(new WinState("the enemy AI"));
		return;
	}
	if (ECSHandler::Instance()->GetComponent<Character>(enemy->GetEntity()).health <= 0)
	{
		Core::Instance()->GetFSM()->ChangeState(new WinState("the player"));
		return;
	}

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
	if (Core::Instance()->KeyDown(SDL_SCANCODE_P))
	{
		Core::Instance()->GetFSM()->PushState(new PauseState());
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
