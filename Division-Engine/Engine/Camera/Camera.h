#ifndef CAMERA_H
#define CAMERA_H

#include "../../DivisionPCH.h"
#include "Frustum.h"
#include "../Rendering/SceneObject.h"

// Encapsulating Projection and View matrices.			
// Also maintains a Frustum to perform culling against. 
class Camera : public SceneObject {
public:
	Camera();
	~Camera();

	//FOVY in degrees
	void SetProjectionMatrix(float fovy_, float aspectRatio_, float near_, float far_);
	virtual void WindowResizeCallback(const int width_, const int height_);

	const glm::mat4& GetViewMatrix();
	const glm::mat4& GetProjectionMatrix();
	inline Frustum& GetFrustum() { return frustum; }

	inline bool ChangedSinceLastCall() { bool temp = dirty; dirty = false; return temp; }

protected:
	glm::mat4 projectionMatrix;
	bool projectionMatrixDirty;
	bool dirty;
	Frustum frustum;
};

#endif



