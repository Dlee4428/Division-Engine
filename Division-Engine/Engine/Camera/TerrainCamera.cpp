#include "TerrainCamera.h"

TerrainCamera::TerrainCamera(float vel_) : prevCursorPos(maxInt), vectorMovement(0), vel(vel_), lateVel(vel_ / 10.0f) {
}

TerrainCamera::~TerrainCamera()
{
}

void TerrainCamera::Update(float deltaTime_)
{
	// This is CONDITIONAL OPERATOR SHORTHAND FOR
	// IF / THEN / ELSE
	// IF a is true, return b, else return c
	float initVel = vectorMovement.z ? lateVel : vel;

	// Update Camera Movement
	if (vectorMovement.x < 0) {
		ProceedForward(-vectorMovement.x * initVel * deltaTime_);
	}
	else if (vectorMovement.x > 0) {
		ProceedBackward(vectorMovement.x * initVel * deltaTime_);
	}

	if (vectorMovement.y < 0) {
		ProceedLeft(vectorMovement.y * initVel * deltaTime_);
	}
	else if (vectorMovement.y > 0) {
		ProceedRight(vectorMovement.y * initVel * deltaTime_);
	}
}

void TerrainCamera::SetInitPosAndRot(const glm::vec3& pos_, const glm::vec3& rot_)
{
	// Set Camera Initial Position and Rotation into the WORLD Coords
	transform.SetPosition(pos_.x, pos_.y, pos_.z, WORLD);
	transform.SetRotation(rot_.x, rot_.y, rot_.z, WORLD, LOCAL);

	const glm::mat4& getTransform = transform.GetTransformationMatrix();
	frustum.SetCameraData(pos_, -transform.GetLocalZVector(), transform.GetLocalYVector());

	dirty = true;
}

void TerrainCamera::ProceedForward(float value_, SpaceType spaceType_)
{
	transform.TranslateObj(0.0f, 0.0f, -value_, spaceType_);
	frustum.SetCameraPosition(transform.GetPosition());
	
	dirty = true;
}

void TerrainCamera::ProceedBackward(float value_, SpaceType spaceType_)
{
	transform.TranslateObj(0.0f, 0.0f, value_, spaceType_);
	frustum.SetCameraPosition(transform.GetPosition());
	
	dirty = true;
}

void TerrainCamera::ProceedLeft(float value_, SpaceType spaceType_)
{
	transform.TranslateObj(-value_, 0.0f, 0.0f, spaceType_);
	frustum.SetCameraPosition(transform.GetPosition());
	
	dirty = true;
}

void TerrainCamera::ProceedRight(float value_, SpaceType spaceType_)
{
	transform.TranslateObj(value_, 0.0f, 0.0f, spaceType_);
	frustum.SetCameraPosition(transform.GetPosition());
	
	dirty = true;
}

void TerrainCamera::Yaw(float value_, SpaceType spaceType_)
{
	transform.RotateObj(0.0f, value_, 0.0f, spaceType_, LOCAL);
	frustum.SetCameraOrientation(
		-transform.GetLocalZVector(), 
		transform.GetLocalYVector(), 
		transform.GetLocalXVector());

	dirty = true;
}

void TerrainCamera::Pitch(float value_, SpaceType spaceType_)
{
	transform.RotateObj(value_, 0.0f, 0.0f, spaceType_, LOCAL);
	frustum.SetCameraOrientation(
		-transform.GetLocalZVector(),
		transform.GetLocalYVector(),
		transform.GetLocalXVector());

	dirty = true;
}

void TerrainCamera::Roll(float value_, SpaceType spaceType_)
{
	transform.RotateObj(0.0f, 0.0f, value_, spaceType_, LOCAL);
	frustum.SetCameraOrientation(
		-transform.GetLocalZVector(),
		transform.GetLocalYVector(),
		transform.GetLocalXVector());

	dirty = true;
}

void TerrainCamera::WindowResizeCallback(const int width_, const int height_)
{
	Camera::WindowResizeCallback(width_, height_);
	prevCursorPos.x = prevCursorPos.y = maxInt;
}

void TerrainCamera::KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_)
{
	// KEY RELEASE SET TO 0
	if (action_ == GLFW_RELEASE) {
		if (key_ == 'W' || key_ == 'S') {
			vectorMovement.x = 0;
		}
		if (key_ == 'A' || key_ == 'D') {
			vectorMovement.y = 0;
		}
	}
	// KEY PRESS EVENT SET 1 to each vector coords
	else if (action_ == GLFW_PRESS) {
		if (key_ == 'W') {
			vectorMovement.x = -1;
			printf("W\n");
		}
		else if (key_ == 'S') {
			vectorMovement.x = 1;
			printf("S\n");
		}
		
		if (key_ == 'A') {
			vectorMovement.y = -1;
			printf("A\n");
		}
		else if (key_ == 'D') {
			vectorMovement.y = 1;
			printf("D\n");
		}
	}
	// IF PRESS CONTROL KEY set 1 to z axis vector coords
	if (mode_ == GLFW_MOD_CONTROL) {
		vectorMovement.z = 1;
	}
	else {
		vectorMovement.z = 0;
	}
}

void TerrainCamera::CursorPositionCallback(const double xpos_, const double ypos_)
{
	int xpos, ypos;

	if (leftMouseDown) {
		if (prevCursorPos.x != maxInt) {
			xpos = int(prevCursorPos.x - float(xpos_));
			Yaw(xpos * 0.1f, WORLD);
		}
		if (prevCursorPos.y != maxInt) {
			ypos = int(prevCursorPos.y - float(ypos_));
			Pitch(ypos * 0.1f, LOCAL);
		}
	}
	// Static cast conversion converts between types using a combination of implicit 
	// and user-defined conversions.
	prevCursorPos.x = static_cast<int>(xpos_);
	prevCursorPos.y = static_cast<int>(ypos_);
}

void TerrainCamera::MouseButtonCallback(const int button_, const int action_, const int mode_)
{
	if (button_ == GLFW_MOUSE_BUTTON_LEFT && action_ == GLFW_PRESS) {
		leftMouseDown = true;
	}
	if (button_ == GLFW_MOUSE_BUTTON_LEFT && action_ == GLFW_RELEASE) {
		leftMouseDown = false;
	}
}
