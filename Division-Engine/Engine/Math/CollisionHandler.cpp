#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<GameObject*> CollisionHandler::prevCollisions = std::vector<GameObject*>();
OctTree* CollisionHandler::scenePartition = nullptr;

CollisionHandler::CollisionHandler()
{
}

CollisionHandler::~CollisionHandler()
{
	OnDestroy();
}

CollisionHandler* CollisionHandler::GetInstance()
{
	if (collisionInstance.get() == nullptr) {
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::OnCreate(float worldSize_)
{
	prevCollisions.clear();
	scenePartition = new OctTree(worldSize_);
}

void CollisionHandler::AddGameObject(GameObject* gameobject_)
{
	scenePartition->AddObject(gameobject_);
}

void CollisionHandler::Update(glm::vec2 mousePosition_, int buttonType_)
{
	Ray mouseRay = CollisionDetection::ScreenPosToWorldRay(mousePosition_,
		CoreEngine::GetInstance()->GetScreenSize(),
		CoreEngine::GetInstance()->GetCamera());

	GameObject* hitResult = scenePartition->GetCollision(mouseRay);

	if (hitResult) {
		hitResult->SetHit(true, buttonType_);
	}

	for (auto prev : prevCollisions) {
		if (prev != hitResult && prev != nullptr) {
			prev->SetHit(false, buttonType_);
			prev = nullptr;
		}
	}

	prevCollisions.clear();

	if (hitResult) {
		prevCollisions.push_back(hitResult);
	}
}

void CollisionHandler::OnDestroy()
{
	for (auto entry : prevCollisions) {
		entry = nullptr;
	}
	prevCollisions.clear();

	delete scenePartition;
	scenePartition = nullptr;
}
