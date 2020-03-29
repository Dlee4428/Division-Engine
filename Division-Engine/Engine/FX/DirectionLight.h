#ifndef DIRECTIONLIGHT_H
#define DIRECTIONLIGHT_H

#include "../../DivisionPCH.h"
#include "Light.h"

class DirectionLight : public Light
{
public:
	DirectionLight(float x_, float y_, float z_);
	~DirectionLight();

	inline glm::vec3& GetDirection() { return direction; }
	void SetDirection(float x_, float y_, float z_);

protected:
	glm::vec3 direction;
};


#endif // !DIRECTIONLIGHT_H
