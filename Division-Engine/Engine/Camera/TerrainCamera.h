#ifndef TERRAINCAMERA_H
#define TERRAINCAMERA_H

#include "Camera.h"

class TerrainCamera : public Camera {
public:
	TerrainCamera(float vel_ = 300.0f);
	~TerrainCamera();

	virtual void Update(float deltaTime_);

	void SetInitPosAndRot(const glm::vec3& pos_, const glm::vec3& rot_);

	void ProceedForward(float value_, SpaceType spaceType_ = LOCAL);
	void ProceedBackward(float value_, SpaceType spaceType_ = LOCAL);
	void ProceedLeft(float value_, SpaceType spaceType_ = LOCAL);
	void ProceedRight(float value_, SpaceType spaceType_ = LOCAL);

	// Y axis
	void Yaw(float value_, SpaceType spaceType_ = LOCAL);
	// X axis
	void Pitch(float value_, SpaceType spaceType_ = LOCAL);
	// Z axis
	void Roll(float value_, SpaceType spaceType_ = LOCAL);


	virtual void WindowResizeCallback(const int width_, const int height_);
	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_);
	virtual void CursorPositionCallback(const double xpos_, const double ypos_);
	virtual void MouseButtonCallback(const int button_, const int action_, const int mode_);

private:
	glm::ivec2 prevCursorPos;
	glm::ivec3 vectorMovement;

	float vel, lateVel;
	bool leftMouseDown = false;

	const int maxInt = std::numeric_limits<int>::max();
	const int minInt = std::numeric_limits<int>::min();
};


#endif // !TERRAINCAMERA_H

