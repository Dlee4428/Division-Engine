#include "DirectionLight.h"

DirectionLight::DirectionLight(float x_, float y_, float z_) {
	SetDirection(x_, y_, z_);
}

DirectionLight::~DirectionLight() {
}

void DirectionLight::SetDirection(float x_, float y_, float z_) {
	direction.x = x_;
	direction.y = y_;
	direction.z = z_;

	direction = glm::normalize(direction);
}