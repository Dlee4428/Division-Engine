#ifndef VIEWCAMERA_H
#define VIEWCAMERA_H

#include "../../DivisionPCH.h"
#include "Camera.h"

class ViewCamera : public Camera
{
public:
	ViewCamera();
	~ViewCamera();

	void SetPositionAndLookAt(const glm::vec3& pos_, const glm::vec3& lookAt_, const glm::vec3& up_);

	void SetPosition(float x_, float y_, float z_);
	void SetLookAt(float x_, float y_, float z_);

private:
	glm::vec3 lookAt;
	glm::vec3 up;

	void UpdateVector();
};

#endif // !VIEWCAMERA_H
