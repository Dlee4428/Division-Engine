#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../../../DivisionPCH.h"

enum SpaceType {
	WORLD, 
	LOCAL
};


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

	const glm::mat4& GetTransformationMatrix();
	const glm::mat4& GetInverseTransformationMatrix();
	const glm::mat3& GetNormalMatrix();

private:
	//In world coordinates parameter
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 localAxis; //representing rotation only, mat4 for convenience

	glm::mat4 transformationMatrix; //local to world
	glm::mat4 inverseTransformationMatrix; //world to local
	glm::mat3 normalMatrix;

	//for returns
	glm::vec3 tempVector;
	glm::vec3 tempVector2;
	glm::vec3 tempVector3;

	bool inverseTransformationMatrixDirtyFlag;
	bool transformationMatrixDirtyFlag;
	bool normalMatrixDirtyFlag;
};

#endif // !TRANSFORM_H