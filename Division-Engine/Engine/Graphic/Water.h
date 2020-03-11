#ifndef WATER_H
#define WATER_H

#include "../../DivisionPCH.h"

#include "../Camera/Camera.h"
#include "../Rendering/GameObject.h"

class Water : public GameObject {
public:
	Water();
	~Water();

	virtual void OnCreate();
	virtual void Render(int objectID_);
	virtual void Update(double deltaTime_);

private:

};

#endif // !WATER_H