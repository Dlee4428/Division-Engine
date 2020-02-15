#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "../../DivisionPCH.h"
#include "../Math/Plane.h"

#ifndef PLANE_COUNT 
#define PLANE_COUNT 6
#endif

enum PlaneData {
	NEAR = 0,
	FAR,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

// For setting Frustum Culling using plane.
class Frustum {
public:
	Frustum();
	~Frustum();

	void SetProjectionData(const float near_, const float far_, const float fovy_, const float aspectRatio_);
	void SetCameraData(const glm::vec3& position_, const glm::vec3& view_, const glm::vec3& up_);

	void SetCameraPosition(const glm::vec3& position_);
	void SetCameraOrientation(const glm::vec3& view_, const glm::vec3& up_, const glm::vec3& right_);

	inline const Plane* GetPlanes() { if (planesDirty) UpdatePlanes();  return planes; }
	const float* GetPackedPlaneData(); // Use for send data to shader

	float fovy; //degrees
	float aspectRatio;
	float near, far;
	glm::vec2 nearDimensions;
	glm::vec2 farDimensions;

private:
	//in world coordinates
	glm::vec3 position;
	glm::vec3 view;
	glm::vec3 up;
	glm::vec3 right;

	float packedPlaneData[24]; // 6 times N.x, N.y, N.z and D
	
	Plane planes[PLANE_COUNT]; // Near, Far, Left, Right, Up, Down in order
	bool planesDirty;
	void UpdatePlanes();
};

#endif // !FRUSTUM_H
