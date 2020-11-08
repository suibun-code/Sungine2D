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

Enemy* enemy;
Player* player;

ECSEntity playerHP;
ECSEntity enemyHP;

void TestState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.2f, .2f, .2f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/musikkperautomatikk.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	ResourceManager::LoadTexture("res/img/enemy.png", true, "enemy");
	ResourceManager::LoadTexture("res/img/player.png", true, "player");

	ResourceManager::AddText("PlayerHP", "0", glm::vec2(0.f), ResourceManager::GetFont("playerHP"));
	ResourceManager::AddText("EnemyHP", "0", glm::vec2(0.f), ResourceManager::GetFont("enemyHP"));

	texture = ResourceManager::GetTexture("enemy");
	enemy = new Enemy(texture, glm::vec2(500.f, 200.f));

	texture = ResourceManager::GetTexture("player");
	player = new Player(texture, glm::vec2(200.f, 200.f));

	//playerHP = ECSHandler::Instance()->CreateEntity();
	//ECSHandler::Instance()->GetComponent<EntityData>(playerHP).name = "PlayerECS";
	//ECSHandler::Instance()->AddComponent(playerHP, TransformComponent{ 1.f, 0.f, glm::vec2(Core::Instance()->GetWindowWidth() / 2, (Core::Instance()->GetWindowHeight() / 2) + 25) });
	//ECSHandler::Instance()->AddComponent(playerHP, RenderComponent{ shader, texture, glm::vec3(1.f) });

	std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";

	State::Enter();
}

void TestState::Update(float deltaTime)
{
	//Collisions.
	if (enemy != nullptr)
	{
		if (Collision::CheckCollision(*player, *enemy))
		{
			GameInstance::Instance()->AddLog("Collided!\n");

			if (enemy->GetHealth() > 0)
				enemy->SetHealth(enemy->GetHealth() - 1);
			else
				enemy->SetDestroyed(true);
		}
	}

	if (Core::Instance()->KeyDown(SDL_SCANCODE_A))
	{
		player->SetDirection(-1);
		player->MoveX(deltaTime);
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_D))
	{
		player->SetDirection(1);
		player->MoveX(deltaTime);
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_W))
	{
		player->SetDirection(-1);
		player->MoveY(deltaTime);
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_S))
	{
		player->SetDirection(1);
		player->MoveY(deltaTime);
	}

	if (Core::Instance()->KeyDown(SDL_SCANCODE_H))
		player->SetHealth(50);

	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu);
		return;
	}

	Core::Instance()->GetSystem<TextSystem>()->Update();

	State::Update(deltaTime);
}

void TestState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (enemy != nullptr)
	{
		if (enemy->GetDestroyed() == false)
		{
			enemy->Draw(*renderer);
			//ResourceManager::Texts["EnemyHP"]->ChangeText(std::to_string(enemy->GetHealth()));
			//ResourceManager::Texts["EnemyHP"]->Update(glm::vec2(enemy->GetPosition().x + 5, enemy->GetPosition().y - 25), ResourceManager::GetFont("enemyHP"));
			//ResourceManager::Texts["EnemyHP"]->Draw(*renderer);

			ECSHandler::Instance()->GetComponent<TextComponent>(ResourceManager::Texts["EnemyHP"]).output = std::to_string(enemy->GetHealth()).c_str();
			ECSHandler::Instance()->GetComponent<TransformComponent>(ResourceManager::Texts["EnemyHP"]).position = glm::vec2(enemy->GetPosition().x + 5, enemy->GetPosition().y - 25), ResourceManager::GetFont("enemyHP");
		}
		else
		{
			ResourceManager::ClearText("EnemyHP");
			delete enemy;
			enemy = nullptr;
		}
	}

	player->Draw(*renderer);

	//ResourceManager::Texts["PlayerHP"]->ChangeText(std::to_string(player->GetHealth()));
	//ResourceManager::Texts["PlayerHP"]->Update(glm::vec2(player->GetPosition().x + 5, player->GetPosition().y - 25), ResourceManager::GetFont("playerHP"));
	//ResourceManager::Texts["PlayerHP"]->Draw(*renderer);

	ECSHandler::Instance()->GetComponent<TextComponent>(ResourceManager::Texts["PlayerHP"]).output = std::to_string(player->GetHealth()).c_str();
	ECSHandler::Instance()->GetComponent<TransformComponent>(ResourceManager::Texts["PlayerHP"]).position = glm::vec2(player->GetPosition().x + 5, player->GetPosition().y - 25), ResourceManager::GetFont("playerHP");

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
	if (mEntities.size() > 0)
		for (auto& entity : mEntities)
			ECSHandler::Instance()->DestroyEntity(entity);

	mEntities.clear();

	ResourceManager::ClearEntities();

	std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";

	State::Exit();
}
