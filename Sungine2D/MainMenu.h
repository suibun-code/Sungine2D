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

template<class T>
inline void MainMenu::CleanVector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}

