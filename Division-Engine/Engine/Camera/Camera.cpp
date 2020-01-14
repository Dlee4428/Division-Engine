#include "Camera.h"
#include "../Core/CoreEngine.h"

Camera::Camera() : position(glm::vec3()), perspective(glm::mat4()), orthographic(glm::mat4()), fieldOfView(0.0f), yaw(0.0f), pitch(0.0f), roll(0.0f),
forwardVector(glm::vec3()), rightVector(glm::vec3()), upVector(glm::vec3()), worldUp(glm::vec3()), nearPlane(0.0f), farPlane(0.0f),
subLightSource(std::vector<LightSource*>()) {
	fieldOfView = 45.0f;
	forwardVector = glm::vec3(0.0f, 0.0f, -1.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	worldUp = upVector;
	nearPlane = 2.0f;
	farPlane = 50.0f;
	yaw = -90.0f; // camera usually starts to right

	perspective = glm::perspective(fieldOfView,
		CoreEngine::GetInstance()->GetScreenSize().x /
		CoreEngine::GetInstance()->GetScreenSize().y,
		nearPlane, farPlane);
	orthographic = glm::ortho(0.0f, CoreEngine::GetInstance()->GetScreenSize().x,
		0.0f, CoreEngine::GetInstance()->GetScreenSize().y,
		-1.0f, 1.0f);

	UpdateCameraVectors();
}

Camera::~Camera()
{
	OnDestroy();
}

void Camera::SetPosition(glm::vec3 position_)
{
	position = position_;
	UpdateCameraVectors();
}

void Camera::SetRotation(float yaw_, float pitch_, float roll_)
{
	yaw = yaw_;
	pitch = pitch_;
	roll = roll_;
	UpdateCameraVectors();
}

// 2019-10-15
void Camera::AddLightSource(LightSource* lightSource_)
{
	subLightSource.push_back(lightSource_);
}

glm::mat4 Camera::GetView() const
{
	return glm::lookAt(position, position + forwardVector, upVector);
}

const glm::mat4 Camera::GetPerspective()
{
	perspective = glm::perspective(fieldOfView,
		CoreEngine::GetInstance()->GetScreenSize().x /
		CoreEngine::GetInstance()->GetScreenSize().y,
		nearPlane, farPlane);

	return perspective;
}

const glm::mat4 Camera::GetOrthographic()
{
	orthographic = glm::ortho(0.0f, CoreEngine::GetInstance()->GetScreenSize().x,
		0.0f, CoreEngine::GetInstance()->GetScreenSize().y,
		-1.0f, 1.0f);
	return orthographic;
}

void Camera::ProcessMouseMovement(float xOffset_, float yOffset_)
{
	xOffset_ *= -0.05f;
	yOffset_ *= 0.05f;

	yaw += xOffset_;
	pitch += yOffset_;

	// Limit Cap
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	if (yaw < 0.0f) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseZoom(int y_)
{
	if (y_ < 0 || y_ > 0) {
		position += static_cast<float>(y_)* (forwardVector * 2.0f);
	}
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	forwardVector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forwardVector.y = sin(glm::radians(pitch));
	forwardVector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	forwardVector = glm::normalize(forwardVector);
	rightVector = glm::normalize(glm::cross(forwardVector, worldUp));
	upVector = glm::normalize(glm::cross(rightVector, forwardVector));

}

void Camera::NormalizePlane(float frustum_[6][4], int side_)
{
	float magnitude = (float)sqrt(
		frustum_[side_][NX] * frustum_[side_][NX] +
		frustum_[side_][NY] * frustum_[side_][NY] +
		frustum_[side_][NY] * frustum_[side_][NY]);

	frustum_[side_][NX] /= magnitude;
	frustum_[side_][NY] /= magnitude;
	frustum_[side_][NZ] /= magnitude;
	frustum_[side_][NO] /= magnitude;
}

void Camera::CalculateFrustum()
{
	proj = GetPerspective();
	view = GetView();

	clip[0][0] = view[0][0] * proj[0][0] + view[0][1] * proj[1][0] + view[0][2] * proj[2][0] + view[0][3] * proj[3][0];
	clip[0][1] = view[0][0] * proj[0][1] + view[0][1] * proj[1][1] + view[0][2] * proj[2][1] + view[0][3] * proj[3][1];
	clip[0][2] = view[0][0] * proj[0][2] + view[0][1] * proj[1][2] + view[0][2] * proj[2][2] + view[0][3] * proj[3][2];
	clip[0][3] = view[0][0] * proj[0][3] + view[0][1] * proj[1][3] + view[0][2] * proj[2][3] + view[0][3] * proj[3][3];

	clip[1][0] = view[1][0] * proj[0][0] + view[1][1] * proj[1][0] + view[1][2] * proj[2][0] + view[1][3] * proj[3][0];
	clip[1][1] = view[1][0] * proj[0][1] + view[1][1] * proj[1][1] + view[1][2] * proj[2][1] + view[1][3] * proj[3][1];
	clip[1][2] = view[1][0] * proj[0][2] + view[1][1] * proj[1][2] + view[1][2] * proj[2][2] + view[1][3] * proj[3][2];
	clip[1][3] = view[1][0] * proj[0][3] + view[1][1] * proj[1][3] + view[1][2] * proj[2][3] + view[1][3] * proj[3][3];

	clip[2][0] = view[2][0] * proj[0][0] + view[2][1] * proj[1][0] + view[2][2] * proj[2][0] + view[2][3] * proj[3][0];
	clip[2][1] = view[2][0] * proj[0][1] + view[2][1] * proj[1][1] + view[2][2] * proj[2][1] + view[2][3] * proj[3][1];
	clip[2][2] = view[2][0] * proj[0][2] + view[2][1] * proj[1][2] + view[2][2] * proj[2][2] + view[2][3] * proj[3][2];
	clip[2][3] = view[2][0] * proj[0][3] + view[2][1] * proj[1][3] + view[2][2] * proj[2][3] + view[2][3] * proj[3][3];

	clip[3][0] = view[3][0] * proj[0][0] + view[3][1] * proj[1][0] + view[3][2] * proj[2][0] + view[3][3] * proj[3][0];
	clip[3][1] = view[3][0] * proj[0][1] + view[3][1] * proj[1][1] + view[3][2] * proj[2][1] + view[3][3] * proj[3][1];
	clip[3][2] = view[3][0] * proj[0][2] + view[3][1] * proj[1][2] + view[3][2] * proj[2][2] + view[3][3] * proj[3][2];
	clip[3][3] = view[3][0] * proj[0][3] + view[3][1] * proj[1][3] + view[3][2] * proj[2][3] + view[3][3] * proj[3][3];


	// This will extract the RIGHT side of the frustum
	frustums[RIGHT][NX] = clip[0][3] - clip[0][0];
	frustums[RIGHT][NY] = clip[1][3] - clip[1][0];
	frustums[RIGHT][NZ] = clip[2][3] - clip[2][0];
	frustums[RIGHT][NO] = clip[3][3] - clip[3][0];
	NormalizePlane(frustums, RIGHT);

	// This will extract the RIGHT side of the frustum
	frustums[LEFT][NX] = clip[0][3] + clip[0][0];
	frustums[LEFT][NY] = clip[1][3] + clip[1][0];
	frustums[LEFT][NZ] = clip[2][3] + clip[2][0];
	frustums[LEFT][NO] = clip[3][3] + clip[3][0];
	NormalizePlane(frustums, LEFT);

	// This will extract the BOTTOM side of the frustum
	frustums[BOTTOM][NX] = clip[0][3] + clip[0][1];
	frustums[BOTTOM][NY] = clip[1][3] + clip[1][1];
	frustums[BOTTOM][NZ] = clip[2][3] + clip[2][1];
	frustums[BOTTOM][NO] = clip[3][3] + clip[3][1];
	NormalizePlane(frustums, BOTTOM);

	// This will extract the TOP side of the frustum
	frustums[TOP][NX] = clip[0][3] - clip[0][1];
	frustums[TOP][NY] = clip[1][3] - clip[1][1];
	frustums[TOP][NZ] = clip[2][3] - clip[2][1];
	frustums[TOP][NO] = clip[3][3] - clip[3][1];
	NormalizePlane(frustums, TOP);

	// This will extract the FRONT side of the frustum
	frustums[FRONT][NX] = clip[0][3] - clip[0][2];
	frustums[FRONT][NY] = clip[1][3] - clip[1][2];
	frustums[FRONT][NZ] = clip[2][3] - clip[2][2];
	frustums[FRONT][NO] = clip[3][3] - clip[3][2];
	NormalizePlane(frustums, FRONT);

	// This will extract the BACK side of the frustum
	frustums[BACK][NX] = clip[0][3] + clip[0][2];
	frustums[BACK][NO] = clip[1][3] + clip[1][2];
	frustums[BACK][NZ] = clip[2][3] + clip[2][2];
	frustums[BACK][NO] = clip[3][3] + clip[3][2];
	NormalizePlane(frustums, BACK);
}

bool Camera::PointInFrustum(glm::vec3 modelPosition_)
{
	// Go through all the sides of the frustum
	for (int i = 0; i < 6; i++)
	{
		// Calculate the plane equation and check if the point is behind a side of the frustum
		if (frustums[i][NX] * modelPosition_.x + frustums[i][NY] * modelPosition_.y + frustums[i][NZ] * modelPosition_.z + frustums[i][NO] <= 0)
		{
			// The point was behind a side, so it ISN'T in the frustum
			return false;
		}
	}
	return true;
}

// 2019-10-15
void Camera::OnDestroy()
{
	if (subLightSource.size() > 0) {
		for (auto light : subLightSource) {
			delete light;
			light = nullptr;
		}
		subLightSource.clear();
	}
}
