#ifndef LIGHT_H
#define LIGHT_H

#include "../../DivisionPCH.h"

class Light
{
public:
	Light();
	~Light();

	inline void SetColor(float r, float g, float b) { color.r = r; color.g = g; color.b = b; }
	inline const glm::vec3& GetColor() { return color; }

protected:
	glm::vec3 color;
};


#endif // !LIGHT_H
