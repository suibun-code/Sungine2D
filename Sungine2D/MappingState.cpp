#include "MappingState.h"

void MappingState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	State::Enter();
}

void MappingState::Update(float deltaTime)
{
	State::Update(deltaTime);
}

void MappingState::Render()
{
	State::Render();
}

void MappingState::Exit()
{
	State::Exit();
}
