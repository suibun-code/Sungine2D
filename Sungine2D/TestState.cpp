#include "TestState.h"

//GLEW
#include "glew.h"

#include "Core.h"
#include "GameInstance.h"
#include "ResourceManager.h"  
#include "SuSprite.h"
#include "Player.h"

//SuSprite* pSuSprite;
Player* player;

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
	player = new Player(glm::vec2(200.f, 200.f), myShader);

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

	State::Update(deltaTime);
}

void TestState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	SuTexture2D myTexture;
	myTexture = ResourceManager::GetTexture("player");
	player->DrawSprite(myTexture, player->GetPosition(), glm::vec2(myTexture.Width, myTexture.Height));

	State::Render();
}

void TestState::Exit()
{
	const char* test[] = { "[EXIT] ", " '", mStateName, "'.\n" };
	GameInstance::LogBuffer(test, sizeof(test) / sizeof(test[0]));

	Core::Instance()->GetAM()->ClearMusic();

	//delete pSuSprite;
	delete player;
}
