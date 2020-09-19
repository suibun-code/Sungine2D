#include "TestState.h"

//GLEW
#include "glew.h"

#include "Core.h"

void TestState::Enter()
{
	glClearColor(.294f, .0f, .509f, 1.f);
}

void TestState::Update(float deltaTime)
{
	State::Update(deltaTime);
}

void TestState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	State::Render();
}

void TestState::Exit()
{
}
