#ifndef DIRECTIONLIGHT_H
#define DIRECTIONLIGHT_H

#include "../../DivisionPCH.h"
#include "Light.h"

class DirectionLight : public Light
{
public:
	DirectionLight(float x, float y, float z);
	~DirectionLight();

	inline glm::vec3& GetDirection() { return direction; }
	void SetDirection(float x, float y, float z);

protected:
	glm::vec3 direction;
};


#endif // !DIRECTIONLIGHT_H
