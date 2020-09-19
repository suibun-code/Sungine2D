#include "TestState.h"

//GLEW
#include "glew.h"

#include "Core.h"
#include "ResourceManager.h"
#include "SuSprite.h"
#include "Player.h"

//SuSprite* pSuSprite;
Player* player;

void TestState::Enter()
{
	glClearColor(.294f, .0f, .509f, 1.f);

	ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	glm::mat4 projection = projection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -5.f, 5.f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	ShaderUtil myShader;
	myShader = ResourceManager::GetShader("sprite");

	//pSuSprite = new SuSprite(myShader);
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

	//pSuSprite->DrawSprite(myTexture, glm::vec2(250.0f, 150.0f), glm::vec2(myTexture.Width, myTexture.Height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	player->DrawSprite2(myTexture, player->GetPosition(), glm::vec2(myTexture.Width, myTexture.Height));

	State::Render();
}

void TestState::Exit()
{
	//delete pSuSprite;
	delete player;
}
