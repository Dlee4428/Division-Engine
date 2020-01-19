#ifndef CAMERA_H
#define CAMERA_H


#include "../../DivisionPCH.h"
#include "../Graphic/SkyBox.h"

union SDL_Event;
class Camera {
private:
	SkyBox* skybox;
	glm::mat3 model;
	glm::mat4 projection;
	glm::mat4 view, rotation, translation;
	glm::vec3 pos, at, up;
public:
	inline glm::mat4 getProjectionMatrix() const { return projection; }
	inline glm::mat4 getViewMatrix() const { return view; }
	inline glm::mat3 getModelMatrix() const { return model; }
	inline glm::vec3 getPosition() const { return pos; }

	bool OnCreate();
	void OnDestroy();
	void createProjection(float fovy_, float aspectRatio_, float near_, float far_);
	void createView(glm::vec3 pos_, glm::vec3 at_, glm::vec3 up_);
	void HandleEvents(const SDL_Event& sdlEvent);
	void Update(const float deltaTime_);
	void Render();
	Camera();
	~Camera();
};

#endif



