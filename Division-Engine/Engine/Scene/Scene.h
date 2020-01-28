#ifndef SCENE_H
#define SCENE_H

#include "../Core/CoreEngine.h"

class Scene : public CoreEngine {
public:
	Scene();
	~Scene();

	virtual void OnCreate();
	virtual void Render();
	virtual void Update(const double deltaTime_);

	virtual void WindowResizeCallback(const int width_, const int height_);
	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_);
	virtual void CursorPositionCallback(const double xpos_, const double ypos_);
	virtual void MouseButtonCallback(const int button_, const int action_, const int mode_);

	bool enableShadowMapping;
private:
	int terrainID, skyboxID, sunLightID;
//	FBO* depthFBO;

	void InitFBO();
};

#endif // !SCENE_H
