#pragma once

#include <vector>
#include <string>

//SDL
#include "SDL.h"

#include "SuSpriteRenderer.h"

namespace Sungine
{
	class State
	{
	protected:
		const char* mStateName = "State";

		ShaderUtil shader;
		SuSpriteRenderer* renderer;

	public:
		State(const char* mStateName) : mStateName(mStateName) {}

		virtual void Enter() = 0;
		virtual void Update(float deltaTime);
		virtual void Render();
		virtual void HandleStateEvents(const SDL_Event* event);
		virtual void Resume() = 0;
		virtual void Exit() = 0;

		template <class T>
		void CleanVector(std::vector<T> vec);

		std::string GetStateName() { return mStateName; }
	};

	class StateMachine
	{
	private:
		std::vector<State*> mStates;

	public:
		~StateMachine();
		void Update(float deltaTime);
		void Render();
		void HandleStateEvents(SDL_Event* event);
		void PushState(State* pState);
		void ChangeState(State* pState);
		void PopState();
		void Clean();
		std::vector<State*>& GetStates() { return mStates; }
	};
}