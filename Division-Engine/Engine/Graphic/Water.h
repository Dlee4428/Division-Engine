#ifndef WATER_H
#define WATER_H

#include "../Rendering/GameObject.h"

class Water : public GameObject {
public:
	Water();
	~Water();

	virtual void OnCreate();
	virtual void Render(int objectID_);
	virtual void Update(double deltaTime_);

	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_);


private:
	std::string texLocation;
};

#endif // !WATER_H