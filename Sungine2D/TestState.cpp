#include "TestState.h"

//GLEW
#include "glew.h"

#include "Core.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "Collision.h"

#include "MainMenu.h"
#include "SuSprite.h"

#include "Player.h"
#include "Enemy.h"
#include "SuText.h"

SuSprite* renderer;
Enemy* enemy;
Player* player;

void TestState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.2f, .2f, .2f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/musikkperautomatikk.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ShaderUtil myShader;
	myShader = ResourceManager::GetShader("sprite");

	SuTexture2D myTexture;

	ResourceManager::LoadFont("font/CircularStd-Black.ttf", 14, { 0, 175, 0, 255 }, "playerHP");
	ResourceManager::LoadFont("font/CircularStd-Black.ttf", 14, { 0, 0, 175, 255 }, "enemyHP");
	ResourceManager::LoadTexture("res/img/enemy.png", true, "enemy");
	ResourceManager::LoadTexture("res/img/player.png", true, "player");

	renderer = new SuSprite(myShader);

	ResourceManager::AddText("PlayerHP", "0", glm::vec2(0.f), ResourceManager::GetFont("playerHP"));
	ResourceManager::AddText("EnemyHP", "0", glm::vec2(0.f), ResourceManager::GetFont("enemyHP"));

	myTexture = ResourceManager::GetTexture("enemy");
	enemy = new Enemy(myTexture, glm::vec2(500.f, 200.f));

	myTexture = ResourceManager::GetTexture("player");
	player = new Player(myTexture, glm::vec2(200.f, 200.f));

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
	{
		player->SetHealth(50);
	}

	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu);
	}

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
			ResourceManager::Texts["EnemyHP"]->ChangeText(std::to_string(enemy->GetHealth()));
			ResourceManager::Texts["EnemyHP"]->Update(glm::vec2(enemy->GetPosition().x + 5, enemy->GetPosition().y - 25), ResourceManager::GetFont("enemyHP"));
			ResourceManager::Texts["EnemyHP"]->Draw(*renderer);
		}
		else
		{
			ResourceManager::ClearText("EnemyHP");
			delete enemy;
			enemy = nullptr;
		}
	}

	player->Draw(*renderer);
	ResourceManager::Texts["PlayerHP"]->ChangeText(std::to_string(player->GetHealth()));
	ResourceManager::Texts["PlayerHP"]->Update(glm::vec2(player->GetPosition().x + 5, player->GetPosition().y - 25), ResourceManager::GetFont("playerHP"));
	ResourceManager::Texts["PlayerHP"]->Draw(*renderer);

	State::Render();
}

void TestState::Exit()
{
	ResourceManager::ClearTexts();

	delete renderer;
	delete enemy;
	delete player;
	renderer = nullptr;
	enemy = nullptr;
	player = nullptr;

	Core::Instance()->GetAM()->ClearMusic();

	State::Exit();
}
