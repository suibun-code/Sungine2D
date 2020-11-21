#include "FSM.h"

#include <iostream>

#include "Core.h"
#include "GameInstance.h"

//State
void State::Enter()
{
	const char* msg[] = { "[ENTER] '", mStateName, "'.\n" };
	GameInstance::LogBuffer(msg, sizeof(msg) / sizeof(msg[0]));

	Update(Core::Instance()->GetDeltaTime());
}

void State::Update(float deltaTime)
{
	if (Core::Instance()->GameInstanceEnabled() == true)
		GameInstance::Instance()->Update(deltaTime);
}

void State::Render()
{
	if (Core::Instance()->GameInstanceEnabled() == true)
		GameInstance::Instance()->Render();

	SDL_GL_SwapWindow(Core::Instance()->GetWindow());
}

void State::Exit()
{
	const char* msg[] = { "[EXIT] '", mStateName, "'.\n" };
	GameInstance::LogBuffer(msg, sizeof(msg) / sizeof(msg[0]));

	Core::Instance()->GetAM()->ClearSounds();
	Core::Instance()->GetAM()->ClearMusic();

	//Clear texts map.
	ResourceManager::ClearTexts();

	//Destroy all active entities.
	for (int i = mEntities.size() - 1; i >= 0; i--)
		ECSHandler::Instance()->DestroyEntity(mEntities.at(i));

	std::cout << "\n";

	mEntities.clear();

	ResourceManager::DestroyTextures();
}

void State::AddEntity(ECSEntity entity)
{
	mEntities.push_back(entity);
}

void State::RemoveEntity(ECSEntity entity)
{
	for (ECSEntity i = 0; i < mEntities.size(); i++)
		if (entity == mEntities.at(i))
			mEntities.erase(mEntities.begin() + i);
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

	Core::Instance()->SetCurrentState(pState);

	mStates.push_back(pState);
	pState->Enter();
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