#ifndef SCENE_H
#define SCENE_H

#include "../Core/CoreEngine.h"
#include "../Rendering/Buffers/FrameBufferObject.h"
#include "../Camera/TerrainCamera.h"

class Scene : public CoreEngine {
public:
	TerrainCamera* terrainCamera;

	Scene();
	~Scene();

	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void Render();
	virtual void Update(const double deltaTime_);

	virtual void WindowResizeCallback(const int width_, const int height_);
	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_);
	virtual void CursorPositionCallback(const double xpos_, const double ypos_);
	virtual void MouseButtonCallback(const int button_, const int action_, const int mode_);

	bool isShadowMapping;

private:
	int terrainLoc, skyboxLoc, sunDirectionLoc, waterLoc;
	FrameBufferObject* depthFBO;

	void InitFrameBufferObject();
};

#endif // !SCENE_H
