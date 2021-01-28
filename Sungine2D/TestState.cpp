#include "TestState.h"

//GLEW
#include "glew.h"

//Misc
#include "Core.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Level.h"
#include "PlayerCharacter.h"

//States
#include "EnemyCharacter.h"
#include "MainMenu.h"

ECSEntity player;
ECSEntity test;

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
	//SuTexture2D texture;

	//ResourceManager::LoadTexture("res/img/enemy.png", true, "enemy");
	ResourceManager::LoadTexture("res/img/player.png", true, "char");

	Level levelTest;
	levelTest.Load("res/levels/saved.txt");

	ResourceManager::AddText("PlayerHP", "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 255, 125, 0, 255 });

	//texture = ResourceManager::GetTexture("char");
	//player = ECSHandler::Instance()->CreateEntity();
	//ECSHandler::Instance()->GetComponent<EntityData>(player).name = "Player";
	//ECSHandler::Instance()->GetComponent<EntityData>(player).tag = "Player";
	//ECSHandler::Instance()->AddComponent(player, Transform{ glm::vec2(1200.f, 600.f) });
	//ECSHandler::Instance()->AddComponent(player, Rendering{ shader, texture });
	//ECSHandler::Instance()->AddComponent(player, Movement{ });
	//ECSHandler::Instance()->AddComponent(player, Player{ });
	//ECSHandler::Instance()->AddComponent(player, Collider{ true });

	glm::vec2 spawnLocations[5];
	spawnLocations[0] = glm::vec2(200.f, 60.f);
	spawnLocations[1] = glm::vec2(1000.f, 50.f);
	spawnLocations[2] = glm::vec2(300.f, 65.f);
	spawnLocations[3] = glm::vec2(550.f, 55.f);
	spawnLocations[4] = glm::vec2(600.f, 45.f);

	PlayerCharacter* player = new PlayerCharacter();

	//for (int i = 0; i < 1; i++)
	//{
	EnemyCharacter* enemy = new EnemyCharacter();
	ECSHandler::Instance()->GetComponent<Transform>(enemy->GetEntity()).position = spawnLocations[0];
	//}

	Core::Instance()->GetSystem<TextSystem>()->Init();
	Core::Instance()->GetSystem<MovementSystem>()->Init();
	Core::Instance()->GetSystem<CollisionSystem>()->Init();
	Core::Instance()->GetSystem<RenderSystem>()->Init();

	State::Enter();
}

void TestState::HandleStateEvents(SDL_Event* event)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_H))
		ECSHandler::Instance()->GetComponent<Player>(player).health = 50;
}

void TestState::Update(float deltaTime)
{
	ECSHandler::Instance()->GetComponent<Text>(ResourceManager::Texts["PlayerHP"]).ChangeText(std::to_string(ECSHandler::Instance()->GetComponent<Player>(player).health));

	Core::Instance()->GetSystem<TextSystem>()->Update();
	Core::Instance()->GetSystem<MovementSystem>()->Update(deltaTime);
	Core::Instance()->GetSystem<CollisionSystem>()->Update();

	if (ECSHandler::Instance()->GetComponent<Transform>(player).IsDirty())
		ECSHandler::Instance()->GetComponent<Transform>(ResourceManager::Texts["PlayerHP"]).SetPosition(glm::vec2(ECSHandler::Instance()->GetComponent<Transform>(player).position.x + 5, ECSHandler::Instance()->GetComponent<Transform>(player).position.y - 25));

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
