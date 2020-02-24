#ifndef SUNDIRECTION_H
#define SUNDIRECTION_H

#include "../Rendering/GameObject.h"
#include "../FX/DirectionLight.h"

class SunDirection : public GameObject {
public:
	const float sunDistance = 800.0f;
	
	SunDirection(float x_, float y_, float z_);
	~SunDirection();

	virtual void OnCreate();
	virtual void Update(double deltaTime_);
	virtual void Render(int passIDref_ = 0);

	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mode_);

	inline DirectionLight& GetDirectionLight() { return dLight; }

private:
	DirectionLight dLight;

	void DirectionControl();
};

#endif // !SUNDIRECTION_H
