#include "Water.h"
#include "../Core/CoreEngine.h"

Water::Water()
{

}

Water::~Water()
{
}

void Water::OnCreate()
{
	transform.SetPosition(0.0f, 30.0f, 0.0f, WORLD);
	transform.SetScale(12.5f, 12.5f, 12.5f, WORLD);
	transform.SetRotation(0.0f, 90.0f, 0.0f, LOCAL, LOCAL);
	transform.RotateObj(0.0f, 0.0f, 0.0f, WORLD, WORLD);
}

void Water::Render(int objectID_)
{
	if (objectID_ == 0) {
		material->Bind();

		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		Camera& camera = coreEngine->GetActiveCamera();
		glm::mat3 normalMatrix = glm::mat3(transform.GetTransformMatrix());

		glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));			// PROJ MATRIX
		glUniformMatrix4fv(11, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));				// VIEW MATRIX
		glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(transform.GetTransformMatrix()));		// MODEL MATRIX
		glUniformMatrix3fv(13, 1, GL_FALSE, glm::value_ptr(normalMatrix));							// NORMAL MATRIX

		mesh->RenderIndex();
	}
}

void Water::Update(double deltaTime_)
{
	Camera& camera = coreEngine->GetActiveCamera();
}

void Water::KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_)
{

}
