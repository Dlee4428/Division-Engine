#include "GameScene.h"

GameScene::GameScene() : Scene(), scenePtr(std::vector<SceneGraph*>()), modelPtr(std::vector<Model*>()) {

}

GameScene::~GameScene() {
	OnDestroy();
}

bool GameScene::OnCreate() {
	CoreEngine::GetInstance()->SetCamera(new Camera());
	CoreEngine::GetInstance()->GetCamera()->SetPosition(glm::vec3(0.0f, 1.0f, 10.0f));
	CoreEngine::GetInstance()->GetCamera()->AddLightSource(
		new LightSource(glm::vec3(0.0f, 2.0f, 3.0f), 0.2f, 0.5f, glm::vec3(0.7f, 0.7f, 0.7f)));
	CollisionHandler::GetInstance()->OnCreate(100.0f);

	scenePtr.push_back(new SceneGraph());
	scenePtr.push_back(new SceneGraph());

	modelPtr.push_back(new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("BasicShader")));
	scenePtr[0]->GetInstance()->AddModel(modelPtr[0]);
	modelPtr.push_back(new Model("Resources/Models/Apple.obj", "Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("BasicShader")));
	scenePtr[1]->GetInstance()->AddModel(modelPtr[1]);

	scenePtr[0]->GetInstance()->AddGameObject(new GameObject(modelPtr[0]), "Dice");
	scenePtr[1]->GetInstance()->AddGameObject(new GameObject(modelPtr[1]), "Apple");

	scenePtr[0]->GetInstance()->GetGameObject("Dice")->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	scenePtr[1]->GetInstance()->GetGameObject("Apple")->SetPosition(glm::vec3(0.0f, 3.0f, -10.0f));

	return true;
}

void GameScene::Update(const float deltaTime_) {
	SceneGraph::GetInstance()->Update(deltaTime_);
}

void GameScene::Render() {
	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
	CoreEngine::GetInstance()->GetCamera()->CalculateFrustum();

	if (CoreEngine::GetInstance()->GetCamera()->PointInFrustum(scenePtr[0]->GetInstance()->GetGameObject("Dice")->GetPosition()))
	{
		//std::cout << "\nDice! " << std::endl;
	}
	if (CoreEngine::GetInstance()->GetCamera()->PointInFrustum(scenePtr[1]->GetInstance()->GetGameObject("Apple")->GetPosition()))
	{
		//std::cout << "\nApple! " << std::endl;
	}
}

void GameScene::OnDestroy() {
	if (scenePtr.size() > 0) {
		for (auto m : scenePtr) {
			delete m;
			m = nullptr;
		}
		scenePtr.clear();
	}

	if (modelPtr.size() > 0) {
		for (auto m : modelPtr) {
			delete m;
			m = nullptr;
		}
		modelPtr.clear();
	}
}