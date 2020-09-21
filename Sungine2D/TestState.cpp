#include "TestState.h"

//GLEW
#include "glew.h"

#include "Core.h"
#include "GameInstance.h"
#include "ResourceManager.h"  
#include "SuSprite.h"
#include "Player.h"
#include "Enemy.h"

Enemy* enemy;
SuSprite* text;
Player* player;

void TestState::Enter()
{
	//Print entrance to log.
	const char* test[] = { "[ENTER] ", " '", mStateName, "'.\n" };
	GameInstance::LogBuffer(test, sizeof(test) / sizeof(test[0]));

	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	//Font.
	//mpSDLFont = TTF_OpenFont("font/Roboto-Regular.ttf", 12);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/hydrogen.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ShaderUtil myShader;
	myShader = ResourceManager::GetShader("sprite");

	enemy = new Enemy(glm::vec2(500.f, 200.f), myShader);
	text = new SuSprite(myShader);
	player = new Player(glm::vec2(200.f, 200.f), myShader);

	ResourceManager::LoadTexture("res/img/enemy.png", true, "enemy");
	ResourceManager::LoadTextureFont("hello", true, "text");
	ResourceManager::LoadTexture("res/img/player.png", true, "player");
}

void TestState::Update(float deltaTime)
{
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
		enemy->SetHealth(50);
	}

	State::Update(deltaTime);
}

void TestState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	SuTexture2D myTexture;

	myTexture = ResourceManager::GetTexture("enemy");
	enemy->DrawSprite(myTexture, enemy->GetPosition(), glm::vec2(myTexture.Width, myTexture.Height));

	myTexture = ResourceManager::GetTexture("text");
	text->DrawSprite(myTexture, glm::vec2(enemy->GetPosition().x + 5, enemy->GetPosition().y - 25), glm::vec2(myTexture.Width, myTexture.Height));

	myTexture = ResourceManager::GetTexture("player");
	player->DrawSprite(myTexture, player->GetPosition(), glm::vec2(myTexture.Width, myTexture.Height));

	std::string s = std::to_string(enemy->GetHealth());
	char const* c = s.c_str();
	ResourceManager::LoadTextureFont(c, true, "text");

	State::Render();
}

void TestState::Exit()
{
	const char* test[] = { "[EXIT] ", " '", mStateName, "'.\n" };
	GameInstance::LogBuffer(test, sizeof(test) / sizeof(test[0]));

	Core::Instance()->GetAM()->ClearMusic();

	//TTF_CloseFont(mpSDLFont);

	delete enemy;
	delete text;
	delete player;
}
