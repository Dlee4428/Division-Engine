#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine() : window(nullptr), isRunning(false), fps(120), gameInterface(nullptr), currentScene(0), camera(nullptr) {
}

CoreEngine::~CoreEngine() {
	OnDestroy();
}

bool CoreEngine::OnCreate(std::string name_, int width_, int height_) {
	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Window creation failed", "CoreEngine.cpp", __LINE__);
		return isRunning = false;
	}

	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);

	MouseEventListener::RegisterEngineObject(this);

	ShaderHandler::GetInstance()->CreateProgram("ColourShader",
		"./Resources/Shaders/ColourVertexShader.glsl",
		"./Resources/Shaders/ColourFragmentShader.glsl");

	ShaderHandler::GetInstance()->CreateProgram("BasicShader",
		"./Resources/Shaders/VertexShader.glsl",
		"./Resources/Shaders/FragmentShader.glsl");

	if (gameInterface) {
		if (!gameInterface->OnCreate()) {
			// Debug Line
			Debug::FatalError("GameInterface creation failed", "CoreEngine.cpp", __LINE__);
			return isRunning = false;
		}
	}

	Debug::Info("Everything is fine", "CoreEngine.cpp", __LINE__);
	timer.Start();

	return isRunning = true;
}

void CoreEngine::Run() {
	while (isRunning) {
		EventListener::Update();
		timer.UpdateFrameTicks();
		HandleEvents();
		Update(timer.GetDeltaTime());
		Render();
		SDL_Delay(timer.GetSleepTime(fps));
	}
	if (!isRunning) {
		OnDestroy();
	}
}

void CoreEngine::HandleEvents()
{

}

bool CoreEngine::GetIsRunning() const {
	return isRunning;
}

void CoreEngine::SetIsRunning(bool isRunning_) {
	isRunning = isRunning_;
}

void CoreEngine::Update(const float deltaTime_) {
	if (gameInterface) {
		gameInterface->Update(deltaTime_);
	}
}

void CoreEngine::Render() {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //Black background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//CALL GAME RENDER
	if (gameInterface) {
		gameInterface->Render();
	}

	SDL_GL_SwapWindow(window->GetWindow());
}

void CoreEngine::OnDestroy() {
	delete camera;
	camera = nullptr;

	delete gameInterface;
	gameInterface = nullptr;

	delete window;
	window = nullptr;

	SDL_Quit();
	exit(0);
}

CoreEngine* CoreEngine::GetInstance() {
	if (engineInstance.get() == nullptr) {
		engineInstance.reset(new CoreEngine);
	}
	return engineInstance.get();
}

void CoreEngine::SetGameInterface(GameInterface* gameInterface_) {
	gameInterface = gameInterface_;
}

int CoreEngine::GetCurrentScene() const {
	return currentScene;
}

void CoreEngine::SetCurrentScene(int currentScene_) {
	currentScene = currentScene_;
}

glm::vec2 CoreEngine::GetScreenSize() const
{
	return glm::vec2(window->GetWidth(), window->GetHeight());
}

void CoreEngine::NotifyMousePressed(int x_, int y_)
{

}

void CoreEngine::NotifyMouseReleased(int x_, int y_, int buttonType_)
{
	CollisionHandler::GetInstance()->Update(glm::vec2(x_, y_), buttonType_);
}

void CoreEngine::NotifyMouseMove(int x_, int y_)
{
	if (camera) {
		camera->ProcessMouseMovement(MouseEventListener::GetMouseOffset().x, MouseEventListener::GetMouseOffset().y);
	}
}

void CoreEngine::NotifyMouseScroll(int y_)
{
	if (camera) {
		camera->ProcessMouseZoom(y_);
	}
}

void CoreEngine::ExitGame()
{
	isRunning = false;
}

void CoreEngine::SetCamera(Camera* camera_)
{
	camera = camera_;
}
