#include "MainMenu.h"

//GLEW
#include "glew.h"

#include "Core.h"
#include "GameInstance.h"
#include "ShaderUtil.h"
#include "SuSprite.h"
#include "Entity.h"
#include "ResourceManager.h"

#include "TestState.h"

SuSprite* renderer1;
SuText* textenter;
Entity* logo;

void MainMenu::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/CornerOfMemories.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ShaderUtil myShader;
	myShader = ResourceManager::GetShader("sprite");

	SuTexture2D myTexture;
	SuFont myFont;

	ResourceManager::LoadFont("font/CircularStd-Medium.ttf", 14, { 0, 0, 0, 255 }, "CircularMedium");
	ResourceManager::LoadTexture("res/img/sunginelogo.png", true, "logo");

	renderer1 = new SuSprite(myShader);

	myTexture = ResourceManager::GetTexture("logo");
	logo = new Entity(myTexture, glm::vec2((Core::Instance()->GetWindowWidth() / 2) - (myTexture.Width / 2), (Core::Instance()->GetWindowHeight() / 2) - (myTexture.Height / 2)));

	textenter = new SuText("Press Enter To Start", glm::vec2(Core::Instance()->GetWindowWidth() / 2, (Core::Instance()->GetWindowHeight() / 2) + 25), ResourceManager::GetFont("CircularMedium"));

	State::Enter();
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

	logo->Draw(*renderer1);

	textenter->Draw(*renderer1);

	State::Render();
}

void MainMenu::Exit()
{
	Core::Instance()->GetAM()->ClearMusic();

	delete textenter;
	delete renderer1;
	delete logo;
	textenter = nullptr;
	renderer1 = nullptr;
	logo = nullptr;

	State::Exit();
}
