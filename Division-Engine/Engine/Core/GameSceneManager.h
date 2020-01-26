#ifndef GAMESCENEMANAGER_H
#define GAMESCENEMANAGER_H

#include "../../DivisionPCH.h"
#include "../Camera/Camera.h"
#include "../Graphic/Skybox.h"
#include "../Rendering/SceneObject.h"

enum SCENE_NUMBER {
	SCENE0 = 0,
	SCENE1,
	SCENE2
};

class GameSceneManager {
public:
	GameSceneManager();
	~GameSceneManager();

	virtual bool Init(std::string name_, int width_, int height_);
	virtual void Run();
	virtual void Render();
	
	void HandleEvents();
	void AddCamera(Camera* camera_);
	void AddSceneObject(SceneObject* sceneObject_);


	inline Camera& GetCamera(int id_) const { return *camera[id_]; }
	inline Camera& GetActiveCamera() const { return *camera[activeCamera]; }
	inline void SetActiveCamera(int id_) { activeCamera = id_; }

private:
	class Scene* currentScene;
	class Timer* timer;
	class Window* window;

	std::vector<Camera*> camera;
	std::vector<SceneObject*> sceneObjects;

	int activeCamera;
	unsigned int fps;
	bool isRunning;

	Scene* BuildScene(SCENE_NUMBER scene_);
};

#endif // !GAMESCENEMANAGER_H