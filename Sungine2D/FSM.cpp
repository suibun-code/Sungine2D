#include "FSM.h"

#include <iostream>

#include "Core.h"
#include "GameInstance.h"

using namespace Sungine;

//State
void State::Update(float deltaTime)
{
	//if (Core::Instance()->KeyDown(SDL_SCANCODE_GRAVE))
	//	if (Core::Instance()->GameInstanceEnabled() == false)
	//		Core::Instance()->SetGameInstance(true);
	//	else
	//		Core::Instance()->SetGameInstance(false);

	if (Core::Instance()->GameInstanceEnabled() == true)
		GameInstance::Instance()->Update(deltaTime);
}

void State::Render()
{
	if (Core::Instance()->GameInstanceEnabled() == true)
		GameInstance::Instance()->Render();

	SDL_GL_SwapWindow(Core::Instance()->GetWindow());
}

void State::HandleStateEvents(const SDL_Event* event)
{
	if (Core::Instance()->GameInstanceEnabled() == true)
	GameInstance::Instance()->HandleEvents(event);
}
//End State

template<class T>
void State::CleanVector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}

//StateMachine
void StateMachine::Update(float deltaTime)
{
	if (!mStates.empty())
		mStates.back()->Update(deltaTime);
}

void StateMachine::Render()
{
	if (!mStates.empty())
		mStates.back()->Render();
}

void StateMachine::HandleStateEvents(SDL_Event* event)
{
	if (!mStates.empty())
		mStates.back()->HandleStateEvents(event);
}

void StateMachine::PushState(State* pState)
{
	mStates.push_back(pState);
	mStates.back()->Enter();
}

void StateMachine::ChangeState(State* pState)
{
	while (!mStates.empty())
	{
		mStates.back()->Exit();
		delete mStates.back();
		mStates.back() = nullptr;
		mStates.pop_back();
	}
	pState->Enter();
	mStates.push_back(pState);
}

void StateMachine::PopState()
{
	if (!mStates.empty())
	{
		mStates.back()->Exit();
		delete mStates.back();
		mStates.back() = nullptr;
		mStates.pop_back();
	}
	mStates.back()->Resume();
}

void StateMachine::Clean()
{
	while (!mStates.empty())
	{
		mStates.back()->Exit();
		delete mStates.back();
		mStates.back() = nullptr;
		mStates.pop_back();
	}
	std::cout << "state machine cleaned.\n";
}

StateMachine::~StateMachine()
{
	std::cout << "destroying state machine.\n";
	Clean();
}
//End StateMachine