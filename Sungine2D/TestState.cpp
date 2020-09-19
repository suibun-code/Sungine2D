#include "TestState.h"

//GLEW
#include "glew.h"

#include "Core.h"
#include "ResourceManager.h"
#include "SuSprite.h"

SuSprite* pSuSprite;

void TestState::Enter()
{
	glClearColor(.294f, .0f, .509f, 1.f);

	ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	glm::mat4 projection = projection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -5.f, 5.f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	ShaderUtil myShader;
	myShader = ResourceManager::GetShader("sprite");

	pSuSprite = new SuSprite(myShader);

	ResourceManager::LoadTexture("res/img/sample.png", true, "cat");
}

void TestState::Update(float deltaTime)
{
	State::Update(deltaTime);
}

void TestState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	SuTexture2D myTexture;
	myTexture = ResourceManager::GetTexture("cat");

	pSuSprite->DrawSprite(myTexture, glm::vec2(250.0f, 150.0f), glm::vec2(myTexture.Width, myTexture.Height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	State::Render();
}

void TestState::Exit()
{
	delete pSuSprite;
}
