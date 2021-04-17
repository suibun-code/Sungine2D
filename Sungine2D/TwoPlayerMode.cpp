#include "TwoPlayerMode.h"

#include "Core.h"
#include "Level.h"
#include "MainMenu.h"
#include "ParticleGenerator.h"
#include "PauseState.h"
#include "PlayerCharacter.h"
#include "ResourceManager.h"
#include "SecondPlayerCharacter.h"
#include "WinState.h"

SecondPlayerCharacter* player;
SecondPlayerCharacter* player2;

void TwoPlayerMode::Enter()
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

	//PLAYER ONE
	//Particles for the player.
	ParticleGenerator* particles;
	particles = new ParticleGenerator(100, 1, glm::vec4(.0f, .0f, 1.f, 1.f), glm::vec2(10.f, -10.f));
	particles->SetOwner(particles);

	//Player.
	player = new SecondPlayerCharacter(0, glm::vec2(310.f, 360.f));
	player->SetOwner(player);

	//Tell the particles to follow the player.
	particles->FollowEntity(player->GetEntity());

	//PLAYER TWO
	//Particles for the player.
	ParticleGenerator* particles2;
	particles2 = new ParticleGenerator(100, 1, glm::vec4(1.0f, .0f, 1.f, 1.f), glm::vec2(10.f, -10.f));
	particles2->SetOwner(particles2);

	//Player.
	player2 = new SecondPlayerCharacter(1, glm::vec2(970.f, 360.f));
	player2->SetOwner(player2);

	//Tell the particles to follow the player.
	particles2->FollowEntity(player2->GetEntity());

	Core::Instance()->GetSystem<TextSystem>()->Init();
	Core::Instance()->GetSystem<MovementSystem>()->Init();
	Core::Instance()->GetSystem<CollisionSystem>()->Init();
	Core::Instance()->GetSystem<RenderSystem>()->Init();
	Core::Instance()->GetSystem<FollowSystem>()->Init();

	State::Enter();
}

void TwoPlayerMode::HandleStateEvents(SDL_Event* event)
{
}

void TwoPlayerMode::Update(float deltaTime)
{
	Core::Instance()->GetSystem<TextSystem>()->Update();
	Core::Instance()->GetSystem<MovementSystem>()->Update(deltaTime);
	Core::Instance()->GetSystem<CollisionSystem>()->Update();
	Core::Instance()->GetSystem<FollowSystem>()->Update(deltaTime);

	if (ECSHandler::Instance()->GetComponent<Character>(player->GetEntity()).health <= 0)
	{
		Core::Instance()->GetFSM()->ChangeState(new WinState("Player Two (Purple)"));
		return;
	}
	if (ECSHandler::Instance()->GetComponent<Character>(player2->GetEntity()).health <= 0)
	{
		Core::Instance()->GetFSM()->ChangeState(new WinState("Player One (Blue)"));
		return;
	}

	State::Update(deltaTime);
}

void TwoPlayerMode::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw renderable entities.
	Core::Instance()->GetSystem<RenderSystem>()->Draw();

	State::Render();
}

void TwoPlayerMode::LateUpdate(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_P))
	{
		Core::Instance()->GetFSM()->PushState(new PauseState());
		return;
	}
}

void TwoPlayerMode::Exit()
{
	State::Exit();

	Core::Instance()->MoveView(glm::vec3(0.f, 24.f, 0.f));

	std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";
	std::cout << "RenderSystem entity count: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";
}
