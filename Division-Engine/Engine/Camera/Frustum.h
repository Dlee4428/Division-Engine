#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "../../DivisionPCH.h"
#include "../Math/Plane.h"

#ifndef PLANE_DATA 
#define PLANE_DATA 6
#endif

enum FrustumPlanes {
	NEAR = 0,	// NEAR side of the frustum
	FAR = 1,	// FAR side of the frustum
	RIGHT = 2, // RIGHT side of the frustum
	LEFT = 3,	// LEFT side of the frustum
	BOTTOM = 4,	// BOTTOM side of the frustum
	TOP = 5,	// TOP side of the frustum
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

	inline const Plane* GetPlanes() { if (isPlanes) { UpdatePlanes(); } return planes; }
	const float* GetFrustumPlanes(); // Use for sending data to tcs shader for terrain

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
	
	Plane planes[PLANE_DATA]; // Near, Far, Left, Right, Up, Down in order
	bool isPlanes;
	void UpdatePlanes();
};

#endif // !FRUSTUM_H
