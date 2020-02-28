#include "GameObject.h"


GameObject::GameObject() : renderEnabled(true) {
}

GameObject::~GameObject() {
}

void GameObject::OnCreate() {
}

void GameObject::Update(double deltaTime_) {
}

void GameObject::Render(int objectID_) {
	BindMaterial();

	if (mesh != 0) {
		mesh->RenderIndex();
	}
}

void GameObject::BindMaterial() {
	if (material != 0) {
		material->Bind();
	}
}