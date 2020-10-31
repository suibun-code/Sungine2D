#include "MainMenu.h"

//GLEW
#include "glew.h"

#include "Core.h"
#include "GameInstance.h"
#include "ShaderUtil.h"
#include "SuSpriteRenderer.h"
#include "Entity.h"
#include "ResourceManager.h"

#include "TestState.h"

Entity* logo;

void MainMenu::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/CornerOfMemories.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	shader = ResourceManager::GetShader("sprite");

	SuTexture2D texture;

	ResourceManager::LoadFont("font/CircularStd-Medium.ttf", 14, { 0, 0, 0, 255 }, "CircularMedium");
	ResourceManager::LoadTexture("res/img/sunginelogo.png", true, "logo");

	texture = ResourceManager::GetTexture("logo");
	logo = new Entity(texture, glm::vec2((Core::Instance()->GetWindowWidth() / 2) - (texture.Width / 2), (Core::Instance()->GetWindowHeight() / 2) - (texture.Height / 2)));
	logo->SetName("Logo");

	ResourceManager::AddText("LogoText", "Press Enter To Start", glm::vec2(Core::Instance()->GetWindowWidth() / 2, (Core::Instance()->GetWindowHeight() / 2) + 25), ResourceManager::GetFont("CircularMedium"));

	State::Enter();
}

void MainMenu::Update(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_RETURN))
		Core::Instance()->GetFSM()->ChangeState(new TestState);

	State::Update(deltaTime);
}

void MainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	logo->Draw(*renderer);

	for (std::map<std::string, SuText*>::iterator it = ResourceManager::Texts.begin(); it != ResourceManager::Texts.end(); it++)
	{
		it->second->Draw(*renderer);
	}

	State::Render();
}

void MainMenu::Exit()
{
	Core::Instance()->GetAM()->ClearMusic();

	//Destroy text entities and all other entities.
	ResourceManager::ClearTexts();
	ResourceManager::ClearEntities();

	State::Exit();
}
