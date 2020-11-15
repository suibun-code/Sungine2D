#include "TestState.h"

//GLEW
#include "glew.h"

//Misc
#include "Core.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "SuSpriteRenderer.h"
#include "Collision.h"

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
	ResourceManager::LoadTexture("res/img/player.png", true, "player");

	//ResourceManager::AddText("PlayerHP", "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 175, 0, 255 });
	//ResourceManager::AddText("EnemyHP", "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 0, 175, 255 });

	texture = ResourceManager::GetTexture("player");
	player = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->GetComponent<EntityData>(player).name = "Player";
	ECSHandler::Instance()->AddComponent(player, TransformComponent{ 1.f, 0.f, glm::vec2(400.f, 500.f) });
	ECSHandler::Instance()->AddComponent(player, RenderComponent{ shader, texture, glm::vec3(1.f) });
	ECSHandler::Instance()->AddComponent(player, MovementComponent{ });
	ECSHandler::Instance()->AddComponent(player, PlayerComponent{ });
	ECSHandler::Instance()->AddComponent(player, ColliderComponent{ true });

	enemy = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->GetComponent<EntityData>(enemy).name = "Enemy";
	ECSHandler::Instance()->AddComponent(enemy, TransformComponent{ 1.f, 0.f, glm::vec2(300.f, 500.f) });
	ECSHandler::Instance()->AddComponent(enemy, RenderComponent{ shader, texture, glm::vec3(.75f, .5f, .5f) });
	ECSHandler::Instance()->AddComponent(enemy, ColliderComponent{ });

	test = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->GetComponent<EntityData>(test).name = "TestCollisionEntity";
	ECSHandler::Instance()->AddComponent(test, TransformComponent{ 1.f, 0.f, glm::vec2(500.f, 500.f) });
	ECSHandler::Instance()->AddComponent(test, RenderComponent{ shader, texture, glm::vec3(1.f) });
	ECSHandler::Instance()->AddComponent(test, ColliderComponent{ });

	//ECSEntity lol = ECSHandler::Instance()->CreateEntity();
	//ECSHandler::Instance()->AddComponent(lol, TransformComponent{ 1.f, 0.f, glm::vec2(500.f, 500.f) });
	//ECSHandler::Instance()->AddComponent(lol, RenderComponent{ shader, texture, glm::vec3(1.f) });

	//ECSEntity lol1 = ECSHandler::Instance()->CreateEntity();
	//ECSHandler::Instance()->AddComponent(lol1, TransformComponent{ 1.f, 0.f, glm::vec2(500.f, 500.f) });
	//ECSHandler::Instance()->AddComponent(lol1, RenderComponent{ shader, texture, glm::vec3(1.f) });

	//ECSEntity lol2 = ECSHandler::Instance()->CreateEntity();
	//ECSHandler::Instance()->AddComponent(lol2, TransformComponent{ 1.f, 0.f, glm::vec2(500.f, 500.f) });
	//ECSHandler::Instance()->AddComponent(lol2, RenderComponent{ shader, texture, glm::vec3(1.f) });

	//for (int i = 0; i < 100; i++)
	//{
	//	ECSHandler::Instance()->CreateEntity();
	//	ECSHandler::Instance()->AddComponent(mEntities[i], TransformComponent{ 1.f, 0.f, glm::vec2(1.f + (float(rand() % 1280)), (float(rand() % 720))) });
	//	ECSHandler::Instance()->AddComponent(mEntities[i], RenderComponent{ shader, texture, glm::vec3(1.f) });
	//	ECSHandler::Instance()->AddComponent(mEntities[i], MovementComponent{});
	//}

	Core::Instance()->GetSystem<MovementSystem>()->Init();
	Core::Instance()->GetSystem<CollisionSystem>()->Init();
	Core::Instance()->GetSystem<TextSystem>()->Init();
	Core::Instance()->GetSystem<RenderSystem>()->Init();

	State::Enter();
}

void TestState::Update(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_H))
		ECSHandler::Instance()->GetComponent<PlayerComponent>(player).health = 50;

	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu());
		return;
	}

	//ECSHandler::Instance()->GetComponent<TextComponent>(ResourceManager::Texts["EnemyHP"]).output = "100";
	//ECSHandler::Instance()->GetComponent<TransformComponent>(ResourceManager::Texts["EnemyHP"]).position = glm::vec2(ECSHandler::Instance()->GetComponent<TransformComponent>(enemy).position.x + 5, ECSHandler::Instance()->GetComponent<TransformComponent>(enemy).position.y - 25);

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

	//ECSHandler::Instance()->GetComponent<TextComponent>(ResourceManager::Texts["player"]).output = std::to_string(ECSHandler::Instance()->GetComponent<PlayerComponent>(player).health);
	//ECSHandler::Instance()->GetComponent<TransformComponent>(ResourceManager::Texts["player"]).position = glm::vec2(ECSHandler::Instance()->GetComponent<TransformComponent>(player).position.x + 5, ECSHandler::Instance()->GetComponent<TransformComponent>(player).position.y - 25);

	//Draw renderable entities.
	Core::Instance()->GetSystem<RenderSystem>()->Draw();

	State::Render();
}

void TestState::Exit()
{
	Core::Instance()->GetAM()->ClearMusic();

	//Clear texts map.
	ResourceManager::ClearTexts();

	//Destroy all active entities.
	for (int i = mEntities.size() - 1; i >= 0; i--)
	{
		ECSHandler::Instance()->DestroyEntity(mEntities.at(i));
		std::cout << "Destroyed.\n";
	}
	std::cout << "\n";

	mEntities.clear();

	ResourceManager::ClearEntities();
	ResourceManager::DestroyTextures();

	//std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";
	std::cout << "RenderSystem entity count: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";

	State::Exit();
}
