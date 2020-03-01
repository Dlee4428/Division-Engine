#include "Frustum.h"

Frustum::Frustum() : aspectRatio(0), far(0), fovy(0), near(0), planesDirty(true) {
}

Frustum::~Frustum() {
}

void Frustum::SetProjectionData(const float near_, const float far_, const float fovy_, const float aspectRatio_) {
	this->near = near_;
	this->far = far_;
	this->fovy = fovy_;
	this->aspectRatio = aspectRatio_;

	float tg = glm::tan(glm::radians(fovy_ * 0.5f));
	nearDimensions.y = tg * near_ * 2.0f;
	nearDimensions.x = nearDimensions.y * aspectRatio_;

	farDimensions.y = tg * far * 2.0f;
	farDimensions.x = farDimensions.y * aspectRatio_;

	planesDirty = true;
}
void Frustum::SetCameraData(const glm::vec3& position_, const glm::vec3& view_, const glm::vec3& up_) {
	SetCameraPosition(position_);
	SetCameraOrientation(view_, up_, glm::cross(view_, up_));
}

void Frustum::SetCameraPosition(const glm::vec3& position_) {
	this->position = position_;
	planesDirty = true;
}

void Frustum::SetCameraOrientation(const glm::vec3& view_, const glm::vec3& up_, const glm::vec3& right_) {
	this->view = view_;
	this->up = up_;
	this->right = right_;
	planesDirty = true;
}

void Frustum::UpdatePlanes() {
	glm::vec3 nearc = position + (view * near);
	planes[NEAR].Modify(-view, nearc);

	glm::vec3 farc = position + (view * far);
	planes[FAR].Modify(view, farc);

	glm::vec3 a = (nearc + right * nearDimensions.x / 2.0f) - position;
	a = glm::normalize(a);
	glm::vec3 normal = glm::cross(a, up);
	planes[RIGHT].Modify(normal, position);

	a = (nearc - right * nearDimensions.x / 2.0f) - position;
	a = glm::normalize(a);
	normal = glm::cross(up, a);
	planes[LEFT].Modify(normal, position);

	a = (nearc + up * nearDimensions.y / 2.0f) - position;
	a = glm::normalize(a);
	normal = glm::cross(right, a);
	planes[TOP].Modify(normal, position);

	a = (nearc - up * nearDimensions.y / 2.0f) - position;
	a = glm::normalize(a);
	normal = glm::cross(a, right);
	planes[BOTTOM].Modify(normal, position);

	planesDirty = false;
}

const float* Frustum::GetPackedPlaneData() {
	if (planesDirty) {
		UpdatePlanes();
	}

	for (int i = 0; i < 6; ++i) {
		Plane& plane = planes[i];
		float* ptr = packedPlaneData + (i * 4);
		memcpy(ptr, &plane.GetNormal(), 3 * sizeof(float));
		*(ptr + 3) = plane.GetD();
	}

	return packedPlaneData;
}