#ifndef CAMERA_H
#define CAMERA_H

#include "../../DivisionPCH.h"
#include "Frustum.h"
#include "../Rendering/GameObject.h"

// Encapsulating Projection and View matrices.			
// Also maintains a Frustum to perform culling against. 
class Camera : public GameObject {
public:
	Camera();
	~Camera();

	//FOVY in degrees
	void SetProjectionMatrix(float fovy_, float aspectRatio_, float near_, float far_);

	const glm::mat4& GetViewMatrix();
	const glm::mat4& GetProjectionMatrix();
	const glm::mat4& GetInvProjectionMatrix();
	inline Frustum& GetFrustum() { return frustum; }
	inline bool LastUpdate() { bool tmp = isUpdate; isUpdate = false; return tmp; }

	// CallBack Function from GameObject EventHandler
	virtual void WindowResizeCallback(const int width_, const int height_);

protected:
	glm::mat4 projectionMatrix, invProjectionMatrix;
	bool isProjMatrix, isInvProjMatrix;
	bool isUpdate;
	Frustum frustum;
};

#endif



