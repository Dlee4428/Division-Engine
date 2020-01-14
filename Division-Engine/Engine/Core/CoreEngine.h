#ifndef COREENGINE_H
#define COREENGINE_H


#include "../../DivisionPCH.h"

#include "Window.h"
#include "Timer.h"
#include "GameInterface.h"

#include "../Camera/Camera.h"
#include "../Events/EventListener.h"
#include "../Core/Debug/Debug.h"
#include "../Scene/Scene.h"
#include "../Rendering/3D/GameObject.h"
#include "../Graphic/ShaderHandler.h"
#include "../Graphic/TextureHandler.h"
#include "../FX/LightSource.h"
#include "../Rendering/SceneGraph.h"


// Base CoreEngine which is the head class that manages the engine's heart
class CoreEngine
{
public:
	// Singleton design pattern to Intantiate functions for simply calling
	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	// Init all the features inside engine
	bool OnCreate(std::string name_, int width_, int height_);

	// This is the Power button where handles the timer tick and all update and render methods
	void Run(); 
	
	// Handling all the SDL_Events
	void HandleEvents(); 

	bool GetIsRunning() const; 
	void SetIsRunning(bool isRunning_);

	// Singleton Instance Getters
	static CoreEngine* GetInstance();


	void SetGameInterface(GameInterface* gameInterface_);
	int GetCurrentScene() const;
	void SetCurrentScene(int currentScene_);
	glm::vec2 GetScreenSize() const;

	void NotifyMousePressed(int x_, int y_);
	void NotifyMouseReleased(int x_, int y_, int buttonType_);
	void NotifyMouseMove(int x_, int y_);
	void NotifyMouseScroll(int y_);

	void ExitGame();

	void SetCamera(Camera* camera_);
	inline Camera* GetCamera() { return camera; }
private:
	CoreEngine();
	~CoreEngine();
	void Update(const float deltaTime_);
	void Render();
	void OnDestroy();

	Window* window;
	Timer timer;
	GameInterface* gameInterface;
	Camera* camera;

	bool isRunning;
	unsigned int fps;
	int currentScene;

	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>; // Whenever unique_ptr is out of scope this automatically call destructor
};

#endif // !COREENGINE_H

