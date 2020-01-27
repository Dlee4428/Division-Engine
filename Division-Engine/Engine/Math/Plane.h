#ifndef PLANE_H
#define PLANE_H

#include "../../DivisionPCH.h"

/*
* Defines a 3D plane
* A point r = (x, y, z) is on the plane if the vector r - P is perpendicular to N
* N . (r - P) = 0
* N = (A, B, C) = normal of the plane
* P = (px, py, pz) = point on the plane
*
* A(x - px) + B(y - py) + C(z - pz) = 0
* Ax + By + Cz + D = 0
* with D = -Apx - Bpy - Cpz = -N.P
*/
class Plane
{
public:
	Plane();
	Plane(const glm::vec3& normal_, const glm::vec3& point_);
	Plane(const glm::vec3& point1_, const glm::vec3& point2_, const glm::vec3& point3_);
	~Plane();

	void Modify(const glm::vec3& normal_, const glm::vec3& point_);
	void Modify(const glm::vec3& point1_, const glm::vec3& point2_, const glm::vec3& point3_);

	float DistanceToPointAndSide(const glm::vec3& point_) const; //can return negatives meaning point on the opposite side of N

	inline const glm::vec3& GetNormal() const { return N; }
	inline const glm::vec3& GetPoint() const { return P; }
	inline float GetD() const { return D; }

private:
	glm::vec3 N;
	glm::vec3 P;
	float D;
};

#endif // !PLANE_H
