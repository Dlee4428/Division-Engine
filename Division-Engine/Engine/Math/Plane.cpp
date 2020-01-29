#include "Plane.h"

Plane::Plane() : normal(0, 0, 1), point(0), distance(0) {
}

Plane::Plane(const glm::vec3& normal_, const glm::vec3& point_) {
	Modify(normal_, point_);
}

Plane::Plane(const glm::vec3& point1_, const glm::vec3& point2_, const glm::vec3& point3_) {
	Modify(point1_, point2_, point3_);
}

Plane::~Plane() {
}

void Plane::Modify(const glm::vec3& normal_, const glm::vec3& point_) {
	normal = glm::normalize(normal_);
	point = point_;
	distance = glm::dot(-normal, point_);
}

//Expecting CCW winding to compute the correct normal ('outwards' the plane)
void Plane::Modify(const glm::vec3& point1_, const glm::vec3& point2_, const glm::vec3& point3_) {
	glm::vec3 v = point2_ - point1_;
	glm::vec3 u = point3_ - point1_;

	normal = glm::cross(u, v);
	normal = glm::normalize(normal);

	point = point1_;

	distance = glm::dot(-normal, point1_);
}

float Plane::DistanceToPointAndSide(const glm::vec3& point_) const {
	return glm::dot(normal, point_) + distance;
}