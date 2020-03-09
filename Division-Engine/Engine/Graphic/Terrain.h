#ifndef TERRAIN_H
#define TERRAIN_H


#include "../../DivisionPCH.h"

#include "../Camera/Camera.h"
#include "../Rendering/GameObject.h"
#include "../Graphic/SunDirection.h"

class Terrain : public GameObject {
public:
	Terrain(SunDirection* sunDirection_);
	~Terrain();

	virtual void OnCreate();
	virtual void Render(int objectID_);
	virtual void Update(double deltaTime_);

	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_);

private:
	// Pointers
	Camera* camera;
	SunDirection* sunDirection;

	// Variables for Terrain Init
	float scaleDisplacement;
	float tessTriWidth;
	glm::ivec2 pixelGridSize;
	glm::vec2 initSize;
	int patchCount;

	// For Frustum Top view
	glm::mat4 topViewMatrix;

	// For KeyCallBack variables
	bool fogVisible;
	bool wireframeMode;

	// For Terrain Depth pass
	glm::mat4 sunProjMat;
	glm::mat4 shadowBias;
};

#endif // !TERRAIN_H
