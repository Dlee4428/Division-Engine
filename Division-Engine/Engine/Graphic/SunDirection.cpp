#include "SunDirection.h"
#include "../Core/CoreEngine.h"

SunDirection::SunDirection(float x_, float y_, float z_) : dLight(x_, y_, z_)
{
}

SunDirection::~SunDirection()
{
}

void SunDirection::OnCreate()
{
	transform.SetPosition(sunDistance, 0.0f, 0.0f, WORLD);
	transform.SetScale(50.0f, 50.0f, 50.0f, WORLD);
	transform.SetRotation(0.0f, 90.0f, 0.0f, LOCAL, LOCAL);
	transform.RotateObj(0.0f, 0.0f, 25.0f, WORLD, WORLD);
	DirectionControl();
}

void SunDirection::Update(double deltaTime_)
{
}

void SunDirection::Render(int passIDref_)
{
	if (passIDref_ == 0) {
		material->Bind();

		glDisable(GL_DEPTH_TEST);

		Camera& camera = coreEngine->GetActiveCamera();
		glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(
			camera.GetProjectionMatrix() *
			camera.GetViewMatrix() *
			transform.GetTransformationMatrix()));
		
		mesh->RenderIndex();
		glEnable(GL_DEPTH_TEST);
	}
}

void SunDirection::KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_)
{
	const float velocity = 1.0f;

	if (key_ == GLFW_KEY_LEFT) {
		transform.RotateObj(0.0f, 0.0f, velocity, WORLD, WORLD);
		DirectionControl();
	}
	else if (key_ == GLFW_KEY_RIGHT) {
		transform.RotateObj(0.0f, 0.0f, -velocity, WORLD, WORLD);
		DirectionControl();
	}
}

void SunDirection::DirectionControl()
{
	const glm::vec3 direction = -transform.GetPosition();
	dLight.SetDirection(direction.x, direction.y, direction.z);
}
