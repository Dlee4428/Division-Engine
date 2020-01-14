#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"

class GameObject
{
public:
	GameObject(Model* model_);
	GameObject(Model* model_, glm::vec3 position_);
	~GameObject();
	void Update(const float deltaTime_);
	void Render(Camera* camera_);
	void OnDestroy();

	inline glm::vec3 GetPosition() const { return position; };
	inline float GetAngle() const { return angle; };
	inline glm::vec3 GetRotation() const { return rotation; };
	inline glm::vec3 GetScale() const { return scale; };
	inline BoundingBox GetBoundingBox() { return boundingBox; }
	inline std::string GetTag() { return tag; }
	inline bool GetHit() const { return hit; }

	void SetPosition(glm::vec3 position_);
	void SetAngle(float angle_);
	void SetRotation(glm::vec3 rotation_);
	void SetScale(glm::vec3 scale_);
	void SetTag(std::string tag_);
	void SetHit(bool hit_, int buttonType_);
private:
	Model* model;
	glm::vec3 position;
	float angle;
	glm::vec3 rotation;
	glm::vec3 scale;
	std::string tag;

	bool hit;

	int modelInstance;
	BoundingBox boundingBox;
};
#endif // !GAMEOBJECT_H


