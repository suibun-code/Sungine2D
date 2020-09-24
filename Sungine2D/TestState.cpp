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
SuText* text;
SuText* text2;

void TestState::Enter()
{
	//Print entrance to log.
	const char* test[] = { "[ENTER] ", " '", mStateName, "'.\n" };
	GameInstance::LogBuffer(test, sizeof(test) / sizeof(test[0]));

	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/hydrogen.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ShaderUtil myShader;
	myShader = ResourceManager::GetShader("sprite");

	SuTexture2D myTexture;
	SuFont myFont;

	ResourceManager::LoadFont("font/CircularStd-Black.ttf", 14, { 0, 175, 0, 255 }, "playerHP");
	ResourceManager::LoadFont("font/CircularStd-Black.ttf", 14, { 0, 0, 175, 255 }, "enemyHP");
	ResourceManager::LoadTexture("res/img/enemy.png", true, "enemy");
	ResourceManager::LoadTexture("res/img/player.png", true, "player");

	renderer = new SuSprite(myShader);

	myFont = ResourceManager::GetFont("font1");
	text = new SuText();
	text2 = new SuText();

	myTexture = ResourceManager::GetTexture("enemy");
	enemy = new Enemy(myTexture, glm::vec2(500.f, 200.f));

	myTexture = ResourceManager::GetTexture("player");
	player = new Player(myTexture, glm::vec2(200.f, 200.f));
}

void TestState::Update(float deltaTime)
{
	//Collisions.
	if (Collision::CheckCollision(*player, *enemy))
	{
		GameInstance::Instance()->AddLog("Collided!\n");

		if (enemy->GetHealth() >= 0)
			enemy->SetHealth(enemy->GetHealth() - 1);
		else
		{
			enemy->SetDestroyed(true);
			delete enemy;
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

	if (enemy->GetDestroyed() == false)
	{
		enemy->Draw(*renderer);
		text2->Draw(*renderer, enemy->GetHealth(), glm::vec2(enemy->GetPosition().x + 5, enemy->GetPosition().y - 25), ResourceManager::GetFont("enemyHP"));
	}
		
	player->Draw(*renderer);
	text->Draw(*renderer, player->GetHealth(), glm::vec2(player->GetPosition().x + 5, player->GetPosition().y - 25), ResourceManager::GetFont("playerHP"));

	State::Render();
}

void TestState::Exit()
{
	const char* test[] = { "[EXIT] ", " '", mStateName, "'.\n" };
	GameInstance::LogBuffer(test, sizeof(test) / sizeof(test[0]));

	Core::Instance()->GetAM()->ClearMusic();

	delete renderer;
	delete enemy;
	delete player;
	delete text;
}
