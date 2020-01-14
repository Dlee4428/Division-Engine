#include "StartScene.h"

StartScene::StartScene() : Scene() {

}

StartScene::~StartScene() {
	OnDestroy();
}

bool StartScene::OnCreate() {
	CoreEngine::GetInstance()->SetCurrentScene(1);
	return true;
}

void StartScene::Update(const float deltaTime_) {

}
void StartScene::Render() {

}
void StartScene::OnDestroy() {

}