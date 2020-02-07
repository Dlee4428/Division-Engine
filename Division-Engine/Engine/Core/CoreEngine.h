#ifndef COREENGINE_H
#define COREENGINE_H

#include "../../DivisionPCH.h"
#include "Event/EventHandler.h"
#include "../Camera/Camera.h"
#include "../Rendering/SceneObject.h"

class CoreEngine : public EventHandler {
public:
	CoreEngine();
	~CoreEngine();

	// CoreEngine Base structure
	// Init by OnCreate(), Update(), Render() finally OnDesroy() to clear memory
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void Update(const double deltaTime_);
	virtual void Render();

	// glfw Callback functions using EventHandler Interface
	virtual void WindowResizeCallback(const int width_, const int height_);
	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mods_);
	virtual void CursorPositionCallback(const double xpos_, const double ypos_);
	virtual void mouseButtonCallback(const int button_, const int action_, const int mods_);

	// Create context samples to determine fxaa, SRGB Framebuffer enable status
	bool CreateGLContext(int samples_ = 0, bool srgbFrameBuffer_ = true);
	void CreateTimerLoop();

	// Window Properties and Getters
	void SetWindowProperties(const std::string& title_, const int width_, const int height_);
	inline int GetWindowWidth() const { return windowWidth; }
	inline int GetWindowHeight() const { return windowHeight; }
	inline Camera& GetCamera(int id_) const { return *cameras[id_]; }
	inline void GetActiveCamera(int id_) { activeCamera = id_; }
	inline Camera& GetActiveCamera() const { return *cameras[activeCamera]; }

	// Add Object into CoreEngine
	void AddCamera(Camera* camera_);
	int AddSceneObject(SceneObject* sceneObject_);
	int AddEventHandler(EventHandler* eventHandler_);

protected:
	GLFWwindow* window;

	std::vector<SceneObject*> sceneObjects;
	std::vector<EventHandler*> eventHandler;
	std::vector<Camera*> cameras;
	
	std::string windowTitle;

	int windowWidth, windowHeight;
	int activeCamera;
	double lastFrameTime;
	double lastFPSShowTime;

	bool isCursorEnabled;
	bool vsyncEnabled;
	bool initialized;

	inline void SetVsync() { glfwSwapInterval(vsyncEnabled ? 1 : 0); }
};


#endif // !COREENGINE_H
