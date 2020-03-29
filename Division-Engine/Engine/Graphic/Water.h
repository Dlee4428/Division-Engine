#ifndef WATER_H
#define WATER_H

#include "../../DivisionPCH.h"

#include "../Camera/Camera.h"
#include "../Rendering/GameObject.h"
#include "../Graphic/SunDirection.h"

class Water : public GameObject {
public:
	Water(SunDirection* sunDirection_);
	~Water();

	virtual void OnCreate();
	virtual void Render(int objectID_);
	virtual void Update(double deltaTime_);

	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_);


private:
	// Pointers
	Camera* camera;
	SunDirection* sunDirection;

	std::string texLocation;

	// For Frustum Top view
	glm::mat4 topViewMatrix;

	// For Water Depth pass
	glm::mat4 sunProjMat;
	glm::mat4 shadowBias;
};

#endif // !WATER_H