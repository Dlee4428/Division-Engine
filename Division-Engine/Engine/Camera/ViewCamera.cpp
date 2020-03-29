#include "ViewCamera.h"

ViewCamera::ViewCamera()
{
}

ViewCamera::~ViewCamera()
{
}

void ViewCamera::SetPositionAndLookAt(const glm::vec3& pos_, const glm::vec3& lookAt_, const glm::vec3& up_)
{
	up = up_;
	transform.SetPosition(pos_, WORLD);
	this->lookAt.x = lookAt_.x;
	this->lookAt.y = lookAt_.y;
	this->lookAt.z = lookAt_.z;
	UpdateVector();
}

void ViewCamera::SetPosition(float x_, float y_, float z_)
{
	transform.SetPosition(x_, y_, z_, WORLD);
	UpdateVector();
}

void ViewCamera::SetLookAt(float x_, float y_, float z_)
{
	lookAt.x = x_;
	lookAt.y = y_;
	lookAt.z = z_;

	UpdateVector();
}

void ViewCamera::UpdateVector()
{
	glm::mat4 lookAtView = glm::lookAt(transform.GetPosition(), lookAt, up);

	transform.SetLocalXVector(lookAtView[0].x, lookAtView[1].x, lookAtView[2].x);
	transform.SetLocalYVector(lookAtView[0].y, lookAtView[1].y, lookAtView[2].y);
	transform.SetLocalZVector(lookAtView[0].z, lookAtView[1].z, lookAtView[2].z);

	frustum.SetCameraPosition(transform.GetPosition());
	frustum.SetCameraOrientation(
		-transform.GetLocalZVector(), 
		 transform.GetLocalYVector(), 
		 transform.GetLocalXVector());

	isUpdate = true;
}
