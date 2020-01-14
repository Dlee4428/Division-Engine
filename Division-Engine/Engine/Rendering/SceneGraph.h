#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "../../DivisionPCH.h"
#include "3D/GameObject.h"
#include "../Math/CollisionHandler.h"

class SceneGraph
{
public:
	//Singleton design pattern
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator=(const SceneGraph&) = delete;
	SceneGraph& operator=(SceneGraph&&) = delete;

	SceneGraph();
	~SceneGraph();

	static SceneGraph* GetInstance();
	void AddModel(Model* model_);
	void AddGameObject(GameObject* gameObject_, std::string name_ = "");

	GameObject* GetGameObject(std::string name_);

	void Update(const float deltaTime_);
	void Render(Camera* camera_);
	void OnDestroy();
private:
	static std::unique_ptr<SceneGraph> sceneGraphInstance;
	friend std::default_delete<SceneGraph>;

	static std::map<GLuint, std::vector<Model*>> sceneModels;
	static std::map<std::string, GameObject*> sceneGameObjects;
};

#endif
