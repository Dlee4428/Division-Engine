#include "Camera.h"

Camera::Camera(){
	// Orignal
	//createProjection(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	createProjection(45.0f, 2.0f, 0.5f, 100.0f);
	createView(glm::vec3(0.0, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, -1.0f)); // By doing pos z to 1 makes FPS view
}

Camera::~Camera() {}

bool Camera::OnCreate() {

	return true;
}

// fovy = field of view y
void Camera::OnDestroy() {

}

void Camera::createProjection(float fovy_, float aspect_, float near_, float far_) {
	//projection = MMath::perspective(fovy_, aspect_, near_, far_);
	projection = glm::perspective(fovy_, aspect_, near_, far_);

}

void Camera::createView(glm::vec3 pos_, glm::vec3 at_, glm::vec3 up_) {
	pos = pos_;
	at = at_;
	up = up_;

	view = glm::lookAt(pos_, at_, up_);
}

void Camera::HandleEvents(const SDL_Event& sdlEvent)
{
}

void Camera::Update(const float deltaTime_)
{

}

void Camera::Render()
{

}



