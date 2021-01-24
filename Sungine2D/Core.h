#pragma once

#define FPS 144

#include <memory>
#include <map>

//SDL
#include "SDL.h"

//IMGUi
#include "imgui.h"

//Components
#include "EntityData.h"
#include "Player.h"
#include "Enemy.h"
#include "Transform.h"
#include "Movement.h"
#include "Rendering.h"
#include "Text.h"
#include "Collider.h"

//Systems
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "TextSystem.h"
#include "CollisionSystem.h"
#include "PlayerSystem.h"
#include "EnemySystem.h"
#include "OverlapSystem.h"

#include "FSM.h"
#include "AudioManager.h"

class Core
{
private:
	//Global Core instance.
	static Core* mspInstance;

	std::map<std::string, std::shared_ptr<ECSSystem>> mpSystems;

	//std::shared_ptr<MovementSystem> mpMovementSystem;
	std::shared_ptr<MovementSystem> mpMovementSystem;
	std::shared_ptr<RenderSystem> mpRenderSystem;
	std::shared_ptr<TextSystem> mpTextSystem;
	std::shared_ptr<CollisionSystem> mpCollisionSystem;
	std::shared_ptr<PlayerSystem> mpPlayerSystem;
	std::shared_ptr<EnemySystem> mpEnemySystem;
	std::shared_ptr<OverlapSystem> mpOverlapSystem;

	//OpenGL context.
	SDL_GLContext mGLContext = NULL;

	//Matrix projection.
	glm::mat4 mProjection = glm::mat4(1.f);
	glm::mat4 mView = glm::mat4(1.f);

	//ImGui context.
	ImGuiContext* mImGuiContext = nullptr;

	//SDL event.
	SDL_Event mEvent = SDL_Event();

	//Boolean checks.
	bool mGameInstanceEnabled = false;
	bool mIsRunning = false;
	bool mGotTick = false;
	bool mLMBState = false;
	bool mLMBDown = false;
	bool mMMBState = false;
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
	Sint32 mMouseRelX = 0;
	Sint32 mMouseRelY = 0;

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

	State* mCurrentState = nullptr;

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

	glm::mat4 GetProjectionMatrix() { return mProjection; }
	glm::mat4 GetViewMatrix() { return mView; }
	void MoveView(glm::vec3 translate);

	Sint32 GetMousePosX() { return mMousePosX; }
	Sint32 GetMousePosY() { return mMousePosY; }
	Sint32 GetMouseRelX() { return mMouseRelX; }
	Sint32 GetMouseRelY() { return mMouseRelY; }

	bool IsRunning() { return mIsRunning; }
	bool Tick();
	bool GameInstanceEnabled() { return mGameInstanceEnabled; }
	bool KeyDown(SDL_Scancode k);
	bool KeyUp(SDL_Scancode k);
	bool GetLMBState() { return mLMBState; }
	bool GetLMBDown() { return mLMBDown; }

	void SetGameInstance(bool gameInstanceEnabled) { mGameInstanceEnabled = gameInstanceEnabled; }
	void WaitFor(int time);
	void HandleEvents();
	void Update(float deltaTime);
	void Render();
	void LateUpdate(float deltaTime);
	void Quit();
	void Clean();
	void GetLogicalSize() { SDL_RenderGetLogicalSize(mpSDLRenderer, &mWindowWidth, &mWindowHeight); }
	void SetLMBState(bool state) { mLMBState = state; }

	int GetWindowWidth() { return mWindowWidth; }
	int GetWindowHeight() { return mWindowHeight; }
	int Round64(int num);

	float GetDeltaTime() { return mDeltaTime; }
	float GetFramesPerSecond() { return mFramesPerSecond; }

	void SetCurrentState(State* state) { mCurrentState = state; }
	State* GetCurrentState() { return mCurrentState; }

	template<class T>
	std::shared_ptr<T> GetSystem() { return std::static_pointer_cast<T>(mpSystems[typeid(T).name()]); }
};

