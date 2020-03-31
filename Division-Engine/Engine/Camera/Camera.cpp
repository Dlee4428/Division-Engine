#include "Camera.h"

Camera::Camera() : isUpdate(false), isProjMatrix(false), isInvProjMatrix(false) {
	frustum.SetProjectionData(0.1f, 100.0f, 45.0f, 1.0f);
	frustum.SetCameraData(transform.GetPosition(), -transform.GetLocalZVector(), transform.GetLocalYVector());
}

Camera::~Camera() {

}

// Get View Matrix by InverseTransformation Matrix
const glm::mat4& Camera::GetViewMatrix()
{
	return transform.GetInvTransformMatrix();
}

// Get Projection Matrix with glm::perspective()
const glm::mat4& Camera::GetProjectionMatrix()
{
	if (isProjMatrix) {
		projectionMatrix = glm::perspective(glm::radians(frustum.fovy), frustum.aspectRatio, frustum.near, frustum.far);
		isProjMatrix = false;
	}
	return projectionMatrix;
}

const glm::mat4& Camera::GetInvProjectionMatrix()
{
	if (isInvProjMatrix) {
		invProjectionMatrix = glm::perspective(glm::radians(-frustum.fovy), frustum.aspectRatio, frustum.near, frustum.far);
		isInvProjMatrix = false;
	}
	return invProjectionMatrix;
}

// Frustum Camera Projection Matrix Set
void Camera::SetProjectionMatrix(float fovy_, float aspectRatio_, float near_, float far_)
{
	frustum.SetProjectionData(near_, far_, fovy_, aspectRatio_);
	isProjMatrix = true;
	isInvProjMatrix = true;
	isUpdate = true;
}

// If Resize Window Call ProjMatrix and glViewPort
void Camera::WindowResizeCallback(const int width_, const int height_)
{
	SetProjectionMatrix(frustum.fovy, (float)width_ / (float)height_, frustum.near, frustum.far);
	glViewport(0, 0, width_, height_);
}



