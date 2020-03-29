#include "CoreEngine.h"


// Global CoreEngine Reference
CoreEngine* core;

// Global Callback Inits FOR GLFW
// CoreEngine Key CallBack
void GLFW_KeyCallback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_) {
	core->KeyCallback(key_, scancode_, action_, mods_);
}

// CoreEngine Cursor Position Callback
void GLFW_CursorPositionCallback(GLFWwindow* window_, double xpos_, double ypos_) {
	core->CursorPositionCallback(xpos_, ypos_);
}

// CoreEngine Mouse Button Callback
void GLFW_MouseButtonCallback(GLFWwindow* window_, int button_, int action_, int mods_) {
	core->MouseButtonCallback(button_, action_, mods_);
}

// CoreEngine Frame Buffer Callback
void GLFW_FramebufferCallback(GLFWwindow* window_, int width_, int height_) {
	core->WindowResizeCallback(width_, height_);
}


CoreEngine::CoreEngine() : windowWidth(1200), windowHeight(800), windowTitle("Hello OpenGL Window"),
lastFPSShowTime(0), lastFrameTime(0), window(nullptr), isCursorEnabled(true), vsyncEnabled(true), initialized(false), activeCamera(0) {
	core = this;
}

CoreEngine::~CoreEngine()
{
	OnDestroy();
}

bool CoreEngine::CreateGLContext(int samples_, bool srgbFrameBuffer_)
{
	////////////////////////////
	// GLFW INIT
	////////////////////////////
	if (!glfwInit()) {
		Debug::FatalError("Cannot Init glfw!", __FILE__, __LINE__);
		return false;
	}

	// Determining samples and SRGB enable status
	glfwWindowHint(GLFW_SAMPLES, samples_);
	glfwWindowHint(GLFW_SRGB_CAPABLE, srgbFrameBuffer_ ? GL_TRUE : GL_FALSE);

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	if (!window) {
		Debug::FatalError("Error creating window!", __FILE__, __LINE__);
		return false;
	}

	glfwMakeContextCurrent(window);

	////////////////////////////
	// GLEW INIT
	////////////////////////////
	GLenum error = glewInit();
	if (GLEW_OK != error) {
		std::cout << "Error initing Glew: " << glewGetErrorString(error);
		return false;
	}

	int res;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGetFramebufferAttachmentParameteriv(
		GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &res);

	if (res == GL_LINEAR) {
		std::cout << "Linear RGB Default Framebuffer.\n";
	}
	else if (res == GL_SRGB) {
		std::cout << "sRGB Default Framebuffer.\n";
		glEnable(GL_FRAMEBUFFER_SRGB); // Enable auto Linear RGB to sRGB conversion when writing to sRGB framebuffers
	}

	glGetIntegerv(GL_SAMPLES, &res);
	if (res > 0) {
		std::cout << "Multisampled Default Framebuffer. Samples: " << res << std::endl;
		glEnable(GL_MULTISAMPLE);
	}
	else {
		std::cout << "Non-Multisampled Default Framebuffer.\n";
	}

	//Get Graphic vendor and version
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported: " << version << std::endl;

	glfwSetKeyCallback(window, GLFW_KeyCallback);
	glfwSetCursorPosCallback(window, GLFW_CursorPositionCallback);
	glfwSetMouseButtonCallback(window, GLFW_MouseButtonCallback);
	glfwSetFramebufferSizeCallback(window, GLFW_FramebufferCallback);
	SetVsync();

	return true;
}

void CoreEngine::CreateTimer()
{
	// Timer Loop for GLFW
	double deltaTime = 0, actualTime = 0;
	lastFrameTime = glfwGetTime();
	lastFPSShowTime = 0;

	std::stringstream title;

	while (!glfwWindowShouldClose(window)) {
		actualTime = glfwGetTime();
		deltaTime = actualTime - lastFrameTime;
		lastFrameTime = actualTime;

		if (actualTime - lastFPSShowTime > 1.0) {
			title.str("");
			title << windowTitle << " [FPS: " << (1.0 / deltaTime) << "] [Frame Time: " << deltaTime << "]";
			glfwSetWindowTitle(window, title.str().c_str());
			lastFPSShowTime = actualTime;
		}

		glfwPollEvents();
		Update(deltaTime);
		Render();

		glfwSwapBuffers(window);
	}
}


void CoreEngine::OnDestroy()
{
	if (gameObjects.size() > 0) {
		for (auto obj : gameObjects) {
			delete obj;
			obj = nullptr;
		}
		gameObjects.clear();
	}

	if (cameras.size() > 0) {
		for (auto c : cameras) {
			delete c;
			c = nullptr;
		}
		cameras.clear();
	}
	
	eventHandler.clear();
	glfwTerminate();
}

// SceneObjects Inits pass to here
void CoreEngine::OnCreate()
{
	// SceneObjects OnCreate()
	for (auto object : gameObjects) {
		object->OnCreate();
	}
	initialized = true;
}

void CoreEngine::Render()
{
	for (auto object : gameObjects) {
		if (object->IsRenderEnabled()) {
			object->Render();
		}
	}
}

void CoreEngine::Update(const double deltaTime_)
{
	cameras[activeCamera]->Update(deltaTime_);
	for (auto object : gameObjects) {
		object->Update(deltaTime_);
	}
}

void CoreEngine::WindowResizeCallback(const int width_, const int height_)
{
	if (width_ > 0 && height_ > 0) {
		windowWidth = width_;
		windowHeight = height_;

		for (auto camera : cameras) {
			camera->WindowResizeCallback(width_, height_);
		}
		for (auto et  : eventHandler) {
			et->WindowResizeCallback(width_, height_);
		}
	}
}

// EventHandler GLFW KeyCallBack
void CoreEngine::KeyCallback(const int key_, const int scanCode_, const int action_, const int mods_)
{
	if (key_ == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (key_ == GLFW_KEY_LEFT_CONTROL && action_ == GLFW_PRESS) {
		isCursorEnabled = !isCursorEnabled;
		int value = isCursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(window, GLFW_CURSOR, value);
	}
	else if (key_ == GLFW_KEY_LEFT_ALT && action_ == GLFW_PRESS) {
		vsyncEnabled = !vsyncEnabled;
		SetVsync();
	}

	cameras[activeCamera]->KeyCallback(key_, scanCode_, action_, mods_); // SceneObject EventHandler Public
	for (auto et : eventHandler) {
		et->KeyCallback(key_, scanCode_, action_, mods_);
	}
}

// EventHandler GLFW CursorPosition Callback
void CoreEngine::CursorPositionCallback(const double xpos_, const double ypos_)
{
	cameras[activeCamera]->CursorPositionCallback(xpos_, ypos_);
	for (auto et : eventHandler) {
		et->CursorPositionCallback(xpos_, ypos_);
	}
}

// EventHandler GLFW MouseButton Callback
void CoreEngine::mouseButtonCallback(const int button_, const int action_, const int mods_)
{
	cameras[activeCamera]->MouseButtonCallback(button_, action_, mods_);
	for (auto et : eventHandler) {
		et->MouseButtonCallback(button_, action_, mods_);
	}
}

// Window Properties attribute
void CoreEngine::SetWindowProperties(const std::string& title_, const int width_, const int height_)
{
	windowWidth = width_;
	windowHeight = height_;
	windowTitle = title_;
	glfwSetWindowSize(window, width_, height_);
	glfwSetWindowTitle(window, title_.c_str());

	glViewport(0, 0, windowWidth, windowHeight);
}

void CoreEngine::AddCamera(Camera* camera_)
{
	cameras.push_back(camera_);
}

int CoreEngine::AddGameObject(GameObject* gameObject_)
{
	gameObject_->SetCoreEngine(this);
	gameObjects.push_back(gameObject_);
	return gameObjects.size() - 1;
}

int CoreEngine::AddEventHandler(EventHandler* eventHandler_)
{
	eventHandler.push_back(eventHandler_);
	return eventHandler.size() - 1;
}
