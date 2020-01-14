#include "GameObject.h"

GameObject::GameObject(Model* model_) : model(nullptr), angle(0)
, position(glm::vec3()), rotation(glm::vec3(0.0f, 1.0f, 0.0f)), scale(glm::vec3(1.0f)), modelInstance(0), tag(""), hit(false) {
	model = model_;
	if (model) {
		modelInstance = model->CreateInstance(position, angle, rotation, scale);
		boundingBox = model->GetBoundingBox();
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}

GameObject::GameObject(Model* model_, glm::vec3 position_) : model(nullptr), angle(0)
, position(glm::vec3()), rotation(glm::vec3(0.0f, 1.0f, 0.0f)), scale(glm::vec3(1.0f)), modelInstance(0), tag(""), hit(false) {
	model = model_;
	position = position_;
	if (model) {
		modelInstance = model->CreateInstance(position, angle, rotation, scale);
		boundingBox = model->GetBoundingBox();
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}

GameObject::~GameObject() {
	OnDestroy();
}

void GameObject::Update(const float deltaTime_) {
	SetAngle(angle + 0.005f);
}

void GameObject::Render(Camera* camera_) {
	if (model) {
		model->Render(camera_);
	}
}

void GameObject::OnDestroy() {
	if (model) {
		delete model;
	}
	model = nullptr;
}

void GameObject::SetPosition(glm::vec3 position_)
{
	position = position_;
	if (model) {
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}
void GameObject::SetAngle(float angle_)
{
	angle = angle_;
	if (model) {
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}
void GameObject::SetRotation(glm::vec3 rotation_)
{
	rotation = rotation_;
	if (model) {
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}
void GameObject::SetScale(glm::vec3 scale_)
{
	scale = scale_;
	if (model) {
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
		boundingBox.minVert *= scale.x > 1.0f ? scale : scale / 2.0f;  //if statement before ? scale = true : result
		boundingBox.maxVert *= scale.x > 1.0f ? scale : scale / 2.0f;  //if statement before ? scale = true : result
	}
}

void GameObject::SetTag(std::string tag_) {
	tag = tag_;
}

void GameObject::SetHit(bool hit_, int buttonType_)
{
	hit = hit_;
	if (hit) {
		std::cout << "Hit: " << tag << std::endl;
	}
}
