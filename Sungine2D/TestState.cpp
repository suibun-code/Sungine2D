#include "TestState.h"

//GLEW
#include "glew.h"

//Misc
#include "Core.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Level.h"

//States
#include "MainMenu.h"

ECSEntity player;
ECSEntity enemy;
ECSEntity test;

void TestState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.2f, .2f, .2f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/musikkperautomatikk.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	//ResourceManager::LoadTexture("res/img/enemy.png", true, "enemy");
	ResourceManager::LoadTexture("res/img/player.png", true, "char");

	Level leveltest;
	leveltest.Load("res/levels/first.txt");

	ResourceManager::AddText("PlayerHP", "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 255, 125, 0, 255 });
	ResourceManager::AddText("EnemyHP", "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 0, 175, 255 });

	texture = ResourceManager::GetTexture("char");
	player = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->GetComponent<EntityData>(player).name = "Player";
	ECSHandler::Instance()->AddComponent(player, Transform{ 1.f, 0.f, glm::vec2(500.f, 500.f) });
	ECSHandler::Instance()->AddComponent(player, Rendering{ shader, texture });
	ECSHandler::Instance()->AddComponent(player, Movement{ });
	ECSHandler::Instance()->AddComponent(player, Player{ });
	ECSHandler::Instance()->AddComponent(player, Collider{ true });

	enemy = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->GetComponent<EntityData>(enemy).tag = "Enemy";
	ECSHandler::Instance()->AddComponent(enemy, Transform{ 1.f, 0.f, glm::vec2(200.f, 500.f) });
	ECSHandler::Instance()->AddComponent(enemy, Rendering{ shader, texture, glm::vec3(.75f, .5f, .5f) });
	ECSHandler::Instance()->AddComponent(enemy, Collider{ });

	Core::Instance()->GetSystem<RenderSystem>()->Init();
	Core::Instance()->GetSystem<TextSystem>()->Init();
	Core::Instance()->GetSystem<MovementSystem>()->Init();
	Core::Instance()->GetSystem<CollisionSystem>()->Init();

	State::Enter();
}

void TestState::Update(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_H))
		ECSHandler::Instance()->GetComponent<Player>(player).health = 50;

	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu());
		return;
	}

	ECSHandler::Instance()->GetComponent<Text>(ResourceManager::Texts["PlayerHP"]).output = std::to_string(ECSHandler::Instance()->GetComponent<Player>(player).health);
	ECSHandler::Instance()->GetComponent<Transform>(ResourceManager::Texts["PlayerHP"]).position = glm::vec2(ECSHandler::Instance()->GetComponent<Transform>(player).position.x + 5, ECSHandler::Instance()->GetComponent<Transform>(player).position.y - 25);

	ECSHandler::Instance()->GetComponent<Text>(ResourceManager::Texts["EnemyHP"]).output = "100";
	ECSHandler::Instance()->GetComponent<Transform>(ResourceManager::Texts["EnemyHP"]).position = glm::vec2(ECSHandler::Instance()->GetComponent<Transform>(enemy).position.x + 5, ECSHandler::Instance()->GetComponent<Transform>(enemy).position.y - 25);

	Core::Instance()->GetSystem<TextSystem>()->Update();
	Core::Instance()->GetSystem<MovementSystem>()->Update(deltaTime);
	Core::Instance()->GetSystem<CollisionSystem>()->Update();

	State::Update(deltaTime);
}

void TestState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//if (enemy != nullptr)
	//	if (enemy->GetDestroyed() == false)
	//		enemy->Draw(*renderer);
	//	else
	//	{
	//		ResourceManager::ClearText("EnemyHP");
	//		delete enemy;
	//		enemy = nullptr;
	//	}

	//Draw renderable entities.
	Core::Instance()->GetSystem<RenderSystem>()->Draw();

	State::Render();
}

void TestState::Exit()
{
	State::Exit();

	//std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";
	std::cout << "RenderSystem entity count: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";
}
