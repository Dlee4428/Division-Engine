#include "Scene0.h"
#include "../Core/Debug/Debug.h"
#include "../Camera/Camera.h"

Scene0::Scene0() {

}

Scene0::~Scene0() {
}

bool Scene0::OnCreate() {


	return true;
}

void Scene0::OnDestroy() {

}

void Scene0::HandleEvents(const SDL_Event& sdlEvent) {

}

void Scene0::Update(const float deltaTime_) {
	deltaTime += deltaTime_;
}

void Scene0::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(0);
}