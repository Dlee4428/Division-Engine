#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../DivisionPCH.h"
#include "../Core/CoreEngine.h"

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void OnDestroy();
private:
	std::vector<SceneGraph*> scenePtr;
	std::vector<Model*> modelPtr;

	// SkyBox
	std::vector<std::string> faces
	{
		"./Resources/Textures/CubeMap/posx.jpg",
		"./Resources/Textures/CubeMap/negx.jpg",
		"./Resources/Textures/CubeMap/posy.jpg",
		"./Resources/Textures/CubeMap/negy.jpg",
		"./Resources/Textures/CubeMap/posz.jpg",
		"./Resources/Textures/CubeMap/negz.jpg"
	};
};

#endif // !1

