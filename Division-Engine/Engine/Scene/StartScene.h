#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "../Core/CoreEngine.h"

class StartScene : public Scene
{
public:
	StartScene();
	virtual ~StartScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void OnDestroy();
private:

};

#endif // !STARTSCENE_H

