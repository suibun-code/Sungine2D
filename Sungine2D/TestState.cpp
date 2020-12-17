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
	SuTexture2D texture;

	//ResourceManager::LoadTexture("res/img/enemy.png", true, "enemy");
	ResourceManager::LoadTexture("res/img/player.png", true, "char");

	Level leveltest;
	leveltest.Load("res/levels/saved.txt");

	ResourceManager::AddText("PlayerHP", "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 255, 125, 0, 255 });

	texture = ResourceManager::GetTexture("char");
	player = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->GetComponent<EntityData>(player).name = "Player";
	ECSHandler::Instance()->GetComponent<EntityData>(player).tag = "Player";
	ECSHandler::Instance()->AddComponent(player, Transform{ glm::vec2(500.f, 500.f) });
	ECSHandler::Instance()->AddComponent(player, Rendering{ shader, texture });
	ECSHandler::Instance()->AddComponent(player, Movement{ });
	ECSHandler::Instance()->AddComponent(player, Player{ });
	ECSHandler::Instance()->AddComponent(player, Collider{ true });

	for (int i = 0; i < 11; i++)
	{
		ECSEntity enemy = ECSHandler::Instance()->CreateEntity();
		ECSHandler::Instance()->GetComponent<EntityData>(enemy).tag = "Enemy";
		ECSHandler::Instance()->AddComponent(enemy, Transform{ glm::vec2(200.f + (i * 75), 60.f) });
		ECSHandler::Instance()->AddComponent(enemy, Rendering{ shader, texture, glm::vec3(.75f, .5f, .5f) });
		ECSHandler::Instance()->AddComponent(enemy, Collider{ });
		ECSHandler::Instance()->AddComponent(enemy, Enemy{ });
	}

	Core::Instance()->GetSystem<PlayerSystem>()->Init();
	Core::Instance()->GetSystem<EnemySystem>()->Init();
	Core::Instance()->GetSystem<TextSystem>()->Init();
	Core::Instance()->GetSystem<MovementSystem>()->Init();
	Core::Instance()->GetSystem<CollisionSystem>()->Init();
	Core::Instance()->GetSystem<RenderSystem>()->Init();

	//Core::Instance()->GetSystem<OverlapSystem>()->Init();

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
	Core::Instance()->GetSystem<PlayerSystem>()->Update(deltaTime);
	Core::Instance()->GetSystem<EnemySystem>()->Update(deltaTime);
	Core::Instance()->GetSystem<MovementSystem>()->Update(deltaTime);
	Core::Instance()->GetSystem<CollisionSystem>()->Update();

	if (ECSHandler::Instance()->GetComponent<Transform>(player).IsDirty())
		ECSHandler::Instance()->GetComponent<Transform>(ResourceManager::Texts["PlayerHP"]).SetPosition(glm::vec2(ECSHandler::Instance()->GetComponent<Transform>(player).position.x + 5, ECSHandler::Instance()->GetComponent<Transform>(player).position.y - 25));

	//Core::Instance()->GetSystem<OverlapSystem>()->Update(deltaTime);

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

	//std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";
	//std::cout << "RenderSystem entity count: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";
}
