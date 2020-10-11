#pragma once

#include "FSM.h"

using namespace Sungine;

class MainMenu :  public State
{
private:
	template <class T>
	void CleanVector(std::vector<T> vec);

public:
	MainMenu() : State("MainMenu") {}
	void Enter();
	void Update(float deltaTime);
	void Render();
	void Resume() {}
	void Exit();
};