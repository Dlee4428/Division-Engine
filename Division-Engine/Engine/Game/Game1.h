#ifndef GAME1_H
#define GAME1_H

#include "../Core/CoreEngine.h"
#include "../Scene/StartScene.h"
#include "../Scene/GameScene.h"


class Game1 : public GameInterface {
public:
	Game1();
	virtual ~Game1();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void OnDestroy();
private:
	Scene* currentScene;
	int sceneNum;
	void BuildScene();
};
#endif // !GAME1_H
