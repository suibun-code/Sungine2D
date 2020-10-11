#pragma once

#include "FSM.h"

using namespace Sungine;

class TestState : public State
{
private:
	template <class T>
	void CleanVector(std::vector<T> vec);

public:
	TestState() : State("Test") {}
	void Enter();
	void Update(float deltaTime);
	void Render();
	void Resume() {}
	void Exit();
};
