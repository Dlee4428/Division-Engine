#include "SceneObject.h"


SceneObject::SceneObject() : renderEnabled(true) {
}

SceneObject::~SceneObject() {
}

void SceneObject::OnCreate() {
}

void SceneObject::Update(float deltaTime_) {
}

void SceneObject::Render(int passIDref_) {
	BindMaterial();

	if (mesh != 0) {
		//mesh->RenderIndexed();
	}
}

void SceneObject::BindMaterial() {
	if (material != 0) {
		//material->Bind();
	}
}