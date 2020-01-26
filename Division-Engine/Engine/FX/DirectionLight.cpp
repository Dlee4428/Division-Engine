#include "DirectionLight.h"

DirectionLight::DirectionLight(float x, float y, float z) {
	SetDirection(x, y, z);
}

DirectionLight::~DirectionLight() {
}

void DirectionLight::SetDirection(float x, float y, float z) {
	direction.x = x;
	direction.y = y;
	direction.z = z;

	direction = glm::normalize(direction);
}