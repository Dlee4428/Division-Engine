#include "GameSceneManager.h"
#include "Debug/Debug.h"
#include "Timer.h"
#include "Window.h"
#include "../Scene/Scene0.h"

GameSceneManager::GameSceneManager(): 
	currentScene(nullptr), window(nullptr), timer(nullptr),
	fps(60), isRunning(false) {}

GameSceneManager::~GameSceneManager() {
	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (window) {
		delete window;
		window = nullptr;
	}

	Debug::Info("Deleting the GameSceneManager", __FILE__, __LINE__);
}

bool GameSceneManager::Init(std::string name_, int width_, int height_) {

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize Window object", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}
	
	currentScene = BuildScene(SCENE0);
	if (currentScene == nullptr) {
		Debug::FatalError("Failed to initialize Opening Scene", __FILE__, __LINE__);
		return false;
	}
	return true;
}


void GameSceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		timer->UpdateFrameTicks();
		HandleEvents();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		SDL_GL_SwapWindow(window->GetWindow());
		SDL_Delay(timer->GetSleepTime(fps));
	}
}

void GameSceneManager::HandleEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_EventType::SDL_QUIT:
			isRunning = false;
			return;
		case SDL_EventType::SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym) {
			case SDLK_5:
				printf("Move to Scene0\n");
				currentScene = BuildScene(SCENE0);
				break;
			}
		default:
			currentScene->HandleEvents(sdlEvent);
			break;
		}
	}
}

Scene* GameSceneManager::BuildScene(SCENE_NUMBER scene_) {
	Scene* newScene = nullptr;
	bool status = true; 
	switch (scene_) {
	case SCENE0:  
		newScene = new Scene0();
		status = newScene->OnCreate();
		break;
	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		newScene = nullptr;
		break;
	}
	if (!status) {
		if (newScene) delete newScene;
		return nullptr;
	}
	return newScene;
	
}


