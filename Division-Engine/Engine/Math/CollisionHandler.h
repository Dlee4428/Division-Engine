#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "CollisionDetection.h"
#include "../Core/OctTree.h"

class CollisionHandler {
public:
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator=(const CollisionHandler&) = delete;
	CollisionHandler& operator=(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();
	void OnCreate(float worldSize_);
	void AddGameObject(GameObject* gameobject_);
	void Update(glm::vec2 mousePosition_, int buttonType_);
	void OnDestroy();
private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler> collisionInstance;
	friend std::default_delete<CollisionHandler>;
	static std::vector<GameObject*> prevCollisions; //prev Collision detects the last time event for detected
	static OctTree* scenePartition;
};

#endif