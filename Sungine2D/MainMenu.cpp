#include "MainMenu.h"

//GLEW
#include "glew.h"

#include "Core.h"
#include "GameInstance.h"
#include "ShaderUtil.h"
#include "SuSprite.h"
#include "ResourceManager.h"
#include "TestState.h"

SuSprite* textenter;
SuSprite* pSuSprite;

void MainMenu::Enter()
{
	const char* test[] = { "[ENTER] ", " '", mStateName, "'.\n" };
	GameInstance::LogBuffer(test, sizeof(test) / sizeof(test[0]));

	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/CornerOfMemories.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ShaderUtil myShader;
	myShader = ResourceManager::GetShader("sprite");

	textenter = new SuSprite(myShader);
	pSuSprite = new SuSprite(myShader);

	ResourceManager::LoadTextureFont("Press Enter To Start", true, "enter");
	ResourceManager::LoadTexture("res/img/sunginelogo.png", true, "logo");
}

void MainMenu::Update(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_RETURN))
	{
		Core::Instance()->GetFSM()->ChangeState(new TestState);
	}

	State::Update(deltaTime);
}

void MainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	SuTexture2D myTexture;

	myTexture = ResourceManager::GetTexture("enter");
	textenter->DrawSprite(myTexture, glm::vec2(Core::Instance()->GetWindowWidth() / 2, (Core::Instance()->GetWindowHeight() / 2) + 25), glm::vec2(myTexture.Width, myTexture.Height));

	myTexture = ResourceManager::GetTexture("logo"); 
	pSuSprite->DrawSprite(myTexture, glm::vec2((Core::Instance()->GetWindowWidth() / 2) - (myTexture.Width / 2), (Core::Instance()->GetWindowHeight() / 2) - (myTexture.Height / 2)), glm::vec2(myTexture.Width, myTexture.Height));

	State::Render();
}

void MainMenu::Exit()
{
	const char* test[] = { "[EXIT] ", " '", mStateName, "'.\n" };
	GameInstance::LogBuffer(test, sizeof(test) / sizeof(test[0]));

	Core::Instance()->GetAM()->ClearMusic();

	delete textenter;
	delete pSuSprite;
}
