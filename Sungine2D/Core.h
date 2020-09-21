#pragma once

#define FPS 144

//SDL
#include "SDL.h"

//IMGUi
#include "imgui.h"

#include "FSM.h"
#include "AudioManager.h"

class Core
{
private:

	//Global Core instance.
	static Core* mspInstance;

	//OpenGL context.
	SDL_GLContext mGLContext = NULL;

	//ImGui context.
	ImGuiContext* mImGuiContext = nullptr;

	//SDL event.
	SDL_Event mEvent;

	//Boolean checks.
	bool mGameInstanceEnabled = false;
	bool mIsRunning = false;
	bool mGotTick = false;
	bool mLMBState = false;
	bool mMusicEnabled = false;

	//Time.
	Uint64 mCurrentTime = SDL_GetPerformanceCounter();
	Uint64 mPreviousTime = 0;
	float mDeltaTime = 0;
	float mFramesPerSecond = 0;

	//Unused old variables for capping framerate.
	int mTick = 0;
	int mFrame = 0;
	bool mCap = true;

	//Mouse position.
	Sint32 mMousePosX = 0;
	Sint32 mMousePosY = 0;

	//Window dimensions.
	int mWindowWidth = 1280;
	int mWindowHeight = 720;

	//SDL variables.
	SDL_Window* mpSDLWindow = nullptr;
	SDL_Renderer* mpSDLRenderer = nullptr;
	SDL_Surface* mpSDLSurface = nullptr;

	//Key states.
	const uint8_t* mpKeyStates = nullptr;

	//State machine and audio manager.
	StateMachine* mpFSM = nullptr;
	AudioManager* mpAM = nullptr;

public:
	const char* mVersion = "v0.0.1";

	Core();
	~Core();

	//Singleton instance of Core.
	static Core* Instance()
	{
		if (mspInstance == nullptr)
			mspInstance = new Core();
		return mspInstance;
	}

	//Initialize.
	bool InitAll(const char* title, const int xpos, const int ypos,
		const int width, const int height, const int flags);
	void InitImGui();

	SDL_Event* GetEvent() { return &mEvent; }
	SDL_Window* GetWindow() { return mpSDLWindow; }
	SDL_Renderer* GetRenderer() { return mpSDLRenderer; }

	StateMachine* GetFSM() { return mpFSM; }
	AudioManager* GetAM() { return mpAM; }

	Sint32 GetMousePosX() { return mMousePosX; }
	Sint32 GetMousePosY() { return mMousePosY; }

	bool IsRunning() { return mIsRunning; }
	bool Tick();
	bool GameInstanceEnabled() { return mGameInstanceEnabled; }
	bool KeyDown(SDL_Scancode k);
	bool GetLMBState() { return mLMBState; }

	void SetGameInstance(bool gameInstanceEnabled) { mGameInstanceEnabled = gameInstanceEnabled; }
	void WaitFor(int time);
	void Update(float DeltaTime);
	void Render();
	void HandleEvents();
	void Quit();
	void Clean();
	void GetLocialSize() { SDL_RenderGetLogicalSize(mpSDLRenderer, &mWindowWidth, &mWindowHeight); }
	void SetLMBState(bool state) { mLMBState = state; }

	int GetWindowWidth() { return mWindowWidth; }
	int GetWindowHeight() { return mWindowHeight; }
	int Round64(int num);

	float GetDeltaTime() { return mDeltaTime; }
	float GetFramesPerSecond() { return mFramesPerSecond; }
};
