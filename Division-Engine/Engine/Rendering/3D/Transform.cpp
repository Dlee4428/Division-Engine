#include "Transform.h"

Transform::Transform() : position(0.0f), scale(1.0f), localAxis(1.0f) {
	isTransMatrix = true;
	isInvTransMatrix = true;
	isNormalMatrix = true;
}

Transform::~Transform() {
}

const glm::mat4& Transform::GetTransformMatrix() {
	if (isTransMatrix) {
		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, position);
		transformMatrix *= localAxis;
		transformMatrix = glm::scale(transformMatrix, scale);

		isTransMatrix = false;
	}

	return transformMatrix;
}

const glm::mat4& Transform::GetInvTransformMatrix() {
	if (isInvTransMatrix) {
		invTransformMatrix = glm::mat4(1.0f);
		invTransformMatrix = glm::scale(invTransformMatrix, 1.0f / scale);
		invTransformMatrix *= glm::transpose(localAxis);
		invTransformMatrix = glm::translate(invTransformMatrix, -position);
		isInvTransMatrix = false;
	}

	return invTransformMatrix;
}

const glm::mat3& Transform::GetNormalMatrix() {
	if (isInvTransMatrix) {
		// Uniform scaling -> normal matrix = transformation matrix
		if (scale.x == scale.y && scale.x == scale.z) {
			normalMatrix[0][0] = localAxis[0][0];
			normalMatrix[0][1] = localAxis[0][1];
			normalMatrix[0][2] = localAxis[0][2];

			normalMatrix[1][0] = localAxis[1][0];
			normalMatrix[1][1] = localAxis[1][1];
			normalMatrix[1][2] = localAxis[1][2];

			normalMatrix[2][0] = localAxis[2][0];
			normalMatrix[2][1] = localAxis[2][1];
			normalMatrix[2][2] = localAxis[2][2];
		}
		// else compute transposed inverse of the transformation matrix
		else {
			const glm::mat4& inverse = GetInvTransformMatrix();

			normalMatrix[0][0] = inverse[0][0];
			normalMatrix[0][1] = inverse[1][0];
			normalMatrix[0][2] = inverse[2][0];

			normalMatrix[1][0] = inverse[0][1];
			normalMatrix[1][1] = inverse[1][1];
			normalMatrix[1][2] = inverse[2][1];

			normalMatrix[2][0] = inverse[0][2];
			normalMatrix[2][1] = inverse[1][2];
			normalMatrix[2][2] = inverse[2][2];
		}
		isNormalMatrix = false;
	}

	return normalMatrix;
}

void Transform::SetPosition(float x_, float y_, float z_, SpaceType space_) {
	if (space_ == WORLD) {
		position.x = x_;
		position.y = y_;
		position.z = z_;
	}
	else if (space_ == LOCAL) {
		glm::vec4 pos = GetTransformMatrix() * glm::vec4(x_, y_, z_, 1.0f);
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
	}
	isTransMatrix = true;
	isInvTransMatrix = true;
}

void Transform::SetPosition(const glm::vec3& pos_, SpaceType space_) {
	SetPosition(pos_.x, pos_.y, pos_.z, space_);
}

void Transform::SetRotation(float x_, float y_, float z_, SpaceType space_, SpaceType pivot_) {
	glm::mat4 r = glm::mat4(1.0f);
	r = glm::rotate(r, glm::radians(z_), glm::vec3(0, 0, 1));
	r = glm::rotate(r, glm::radians(x_), glm::vec3(1, 0, 0));
	r = glm::rotate(r, glm::radians(y_), glm::vec3(0, 1, 0));

	if (space_ == WORLD) {
		localAxis = r;
	}
	else if (space_ == LOCAL) {
		localAxis = localAxis * r;
	}

	//Rotation around world origin will change the position
	if (pivot_ == WORLD) {
		glm::mat4 m = glm::mat4(1.0f);
		m[3].x = position.x;
		m[3].y = position.y;
		m[3].z = position.z;
		m = r * m;
		position.x = m[3].x;
		position.y = m[3].y;
		position.z = m[3].z;
	}

	isTransMatrix = true;
	isInvTransMatrix = true;
	isNormalMatrix = true;
}

void Transform::SetScale(float x_, float y_, float z_, SpaceType space_) {
	scale.x = x_;
	scale.y = y_;
	scale.z = z_;

	isTransMatrix = true;
	isInvTransMatrix = true;
	isNormalMatrix = true;
}

void Transform::SetLocalXVector(float x_, float y_, float z_) {
	localAxis[0].x = x_; 
	localAxis[0].y = y_; 
	localAxis[0].z = z_;
	
	isTransMatrix = true;
	isInvTransMatrix = true;
	isNormalMatrix = true;
}

void Transform::SetLocalYVector(float x_, float y_, float z_) {
	localAxis[1].x = x_; 
	localAxis[1].y = y_; 
	localAxis[1].z = z_;
	
	isTransMatrix = true;
	isInvTransMatrix = true;
	isNormalMatrix = true;
}

void Transform::SetLocalZVector(float x_, float y_, float z_) {
	localAxis[2].x = x_; 
	localAxis[2].y = y_; 
	localAxis[2].z = z_;
	
	isTransMatrix = true;
	isInvTransMatrix = true;
	isNormalMatrix = true;
}

const glm::vec3& Transform::GetLocalXVector() { 
	tmpVector.x = localAxis[0].x; 
	tmpVector.y = localAxis[0].y;
	tmpVector.z = localAxis[0].z;
	return tmpVector;
}

const glm::vec3& Transform::GetLocalYVector() { 
	tmpVector2.x = localAxis[1].x; 
	tmpVector2.y = localAxis[1].y;
	tmpVector2.z = localAxis[1].z;
	return tmpVector2;
}

const glm::vec3& Transform::GetLocalZVector() { 
	tmpVector3.x = localAxis[2].x; 
	tmpVector3.y = localAxis[2].y;
	tmpVector3.z = localAxis[2].z;
	return tmpVector3;
}

void Transform::TranslateObj(float x_, float y_, float z_, SpaceType space_) {
	if (space_ == WORLD) {
		position.x += x_;
		position.y += y_;
		position.z += z_;
	}
	else if (space_ == LOCAL) {
		glm::vec4 p = GetTransformMatrix() * glm::vec4(x_, y_, z_, 1.0f);
		position.x = p.x;
		position.y = p.y;
		position.z = p.z;
	}
	isTransMatrix = true;
	isInvTransMatrix = true;
}

void Transform::RotateObj(float x_, float y_, float z_, SpaceType space_, SpaceType pivot_) {

	glm::mat4 r = glm::mat4(1.0f);
	r = glm::rotate(r, glm::radians(z_), glm::vec3(0, 0, 1));
	r = glm::rotate(r, glm::radians(x_), glm::vec3(1, 0, 0));
	r = glm::rotate(r, glm::radians(y_), glm::vec3(0, 1, 0));


	if (space_ == WORLD) {
		localAxis = r * localAxis;
	}
	else if (space_ == LOCAL) {
		localAxis = localAxis * r;
	}

	//Rotation around world origin will change the position
	if (pivot_ == WORLD) {
		glm::mat4 m = glm::mat4(1.0f);
		m[3].x = position.x;
		m[3].y = position.y;
		m[3].z = position.z;
		m = r * m;
		position.x = m[3].x;
		position.y = m[3].y;
		position.z = m[3].z;
	}

	isTransMatrix = true;
	isInvTransMatrix = true;
	isNormalMatrix = true;
}

void Transform::ScaleObj(float x_, float y_, float z_, SpaceType space_) {
	if (space_ == WORLD) {
		
	}
	else if (space_ == LOCAL) {
		scale.x += x_;
		scale.y += y_;
		scale.z += z_;
	}
	isTransMatrix = true;
	isInvTransMatrix = true;
	isNormalMatrix = true;
}