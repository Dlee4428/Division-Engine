#include "Frustum.h"

Frustum::Frustum() : aspectRatio(0), far(0), fovy(0), near(0), isPlanes(true) {
}

Frustum::~Frustum() {
}

void Frustum::SetProjectionData(const float near_, const float far_, const float fovy_, const float aspectRatio_) {
	this->near = near_;
	this->far = far_;
	this->fovy = fovy_;
	this->aspectRatio = aspectRatio_;

	float tangent = glm::tan(glm::radians(fovy_ * 0.5f));
	nearDimensions.y = tangent * near_ * 2.0f;
	nearDimensions.x = nearDimensions.y * aspectRatio_;

	farDimensions.y = tangent * far * 2.0f;
	farDimensions.x = farDimensions.y * aspectRatio_;

	isPlanes = true;
}
void Frustum::SetCameraData(const glm::vec3& position_, const glm::vec3& view_, const glm::vec3& up_) {
	SetCameraPosition(position_);
	SetCameraOrientation(view_, up_, glm::cross(view_, up_));
}

void Frustum::SetCameraPosition(const glm::vec3& position_) {
	this->position = position_;
	isPlanes = true;
}

void Frustum::SetCameraOrientation(const glm::vec3& view_, const glm::vec3& up_, const glm::vec3& right_) {
	this->view = view_;
	this->up = up_;
	this->right = right_;
	isPlanes = true;
}

void Frustum::UpdatePlanes() {
	// GET THE NEAR(FRONT) SIDE OF THE PLANE
	glm::vec3 nearPlane = position + (view * near);
	planes[NEAR].Modify(-view, nearPlane);

	// GET THE FAR(BACK) SIDE OF THE PLANE
	glm::vec3 farPlane = position + (view * far);
	planes[FAR].Modify(view, farPlane);

	// GET THE RIGHT SIDE OF THE PLANE
	glm::vec3 clip = (nearPlane + right * nearDimensions.x / 2.0f) - position;
	clip = glm::normalize(clip);
	glm::vec3 normal = glm::cross(clip, up);
	planes[RIGHT].Modify(normal, position);

	// GET THE LEFT SIDE OF THE PLANE
	clip = (nearPlane - right * nearDimensions.x / 2.0f) - position;
	clip = glm::normalize(clip);
	normal = glm::cross(up, clip);
	planes[LEFT].Modify(normal, position);

	// GET THE BOTTOM SIDE OF THE PLANE
	clip = (nearPlane - up * nearDimensions.y / 2.0f) - position;
	clip = glm::normalize(clip);
	normal = glm::cross(clip, right);
	planes[BOTTOM].Modify(normal, position);

	// GET THE TOP SIDE OF THE PLANE
	clip = (nearPlane + up * nearDimensions.y / 2.0f) - position;
	clip = glm::normalize(clip);
	normal = glm::cross(right, clip);
	planes[TOP].Modify(normal, position);

	isPlanes = false;
}

const float* Frustum::GetFrustumPlanes() {
	if (isPlanes) {
		UpdatePlanes();
	}

	for (int i = 0; i < 6; ++i) {
		Plane& plane = planes[i];
		float* planePtr = packedPlaneData + (i * 4);

		// Copy block of memory
		// Copies the values of num bytes from the location pointed to by source directly 
		// to the memory block pointed to by destination.
		memcpy(planePtr, &plane.GetNormal(), 3 * sizeof(float));
		*(planePtr + 3) = plane.GetDistance(); // Dereference ptr
	}

	return packedPlaneData;
}