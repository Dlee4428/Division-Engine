#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../../../DivisionPCH.h"

enum SpaceType {
	WORLD, 
	LOCAL
};

// USING SEPARATE TRANSFORM CLASS FOR DYNAMIC WAY TO DIVIDE WORLD AND LOCAL LOCATION
class Transform
{
public:
	Transform();
	~Transform();

	void SetPosition(float x_, float y_, float z_, SpaceType space_);
	void SetPosition(const glm::vec3& pos_, SpaceType space_);

	// Param: (Space) Space of the angle values, (pivot) rotation pivot point (WORLD or LOCAL origins)
	void SetRotation(float x_, float y_, float z_, SpaceType space_, SpaceType pivot_);
	void SetScale(float x_, float y_, float z_, SpaceType space_);

	void TranslateObj(float x_, float y_, float z_, SpaceType space_);
	void RotateObj(float x_, float y_, float z_, SpaceType space_, SpaceType pivot_);
	void ScaleObj(float x_, float y_, float z_, SpaceType space_);

	inline const glm::vec3& GetPosition() const { return position; }
	inline const glm::vec3& GetRotation() { return rotation; }
	inline const glm::vec3& GetScale() const { return scale; }

	// Setters
	void SetLocalXVector(float x_, float y_, float z_);
	void SetLocalYVector(float x_, float y_, float z_);
	void SetLocalZVector(float x_, float y_, float z_);

	// Getters
	const glm::vec3& GetLocalXVector();
	const glm::vec3& GetLocalYVector();
	const glm::vec3& GetLocalZVector();

	const glm::mat4& GetTransformMatrix();
	const glm::mat4& GetInvTransformMatrix();
	const glm::mat3& GetNormalMatrix();

private:
	// OpenGL World coords param
	glm::vec3 position;  // Position Object
	glm::vec3 rotation;  // Rotation Object
	glm::vec3 scale;	 // Scale Object
	glm::mat4 localAxis; // For Rotation purposes but to mat4

	glm::mat4 transformMatrix;				// Local to world transformation matrix
	glm::mat4 invTransformMatrix;			// World to local transformation matrix
	glm::mat3 normalMatrix;					// For Normal Matrix

	// For returns
	glm::vec3 tmpVector;
	glm::vec3 tmpVector2;
	glm::vec3 tmpVector3;

	// Dirty Flag design pattern
	// Soruce - https://gameprogrammingpatterns.com/dirty-flag.html
	bool isInvTransMatrix;
	bool isTransMatrix;
	bool isNormalMatrix;
};

#endif // !TRANSFORM_H
