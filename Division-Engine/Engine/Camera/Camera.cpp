#include "Camera.h"

Camera::Camera() : dirtyFlag(false), projectionMatrixDirtyFlag(false) {
	frustum.SetProjectionData(0.1f, 100.0f, 45.0f, 1.0f);
	frustum.SetCameraData(transform.GetPosition(), -transform.GetLocalZVector(), transform.GetLocalYVector());
}

Camera::~Camera() {

}

const glm::mat4& Camera::GetViewMatrix()
{
	return transform.GetInverseTransformationMatrix();
}

const glm::mat4& Camera::GetProjectionMatrix()
{
	if (projectionMatrixDirtyFlag) {
		projectionMatrix = glm::perspective(glm::radians(frustum.fovy), frustum.aspectRatio, frustum.near, frustum.far);
		projectionMatrixDirtyFlag = false;
	}
	return projectionMatrix;
}

void Camera::SetProjectionMatrix(float fovy_, float aspectRatio_, float near_, float far_)
{
	frustum.SetProjectionData(near_, far_, fovy_, aspectRatio_);
	projectionMatrixDirtyFlag = true;
	dirtyFlag = true;
}

void Camera::WindowResizeCallback(const int width_, const int height_)
{
	SetProjectionMatrix(frustum.fovy, (float)width_ / (float)height_, frustum.near, frustum.far);
	glViewport(0, 0, width_, height_);
}



