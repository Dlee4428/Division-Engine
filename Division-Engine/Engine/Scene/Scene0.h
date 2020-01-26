#ifndef SCENE0_H
#define SCENE0_H

#include "../../DivisionPCH.h"
#include "Scene.h"

/// Forward declarations 
union SDL_Event;

class Scene0 : public Scene {
private:
	float deltaTime;
	bool chromatic = false;
public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE0_H