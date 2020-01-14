#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "../../DivisionPCH.h"

// 2019-10-15
class LightSource {
public:
	LightSource(glm::vec3 position_, float ambient_, float diffuse_, glm::vec3 lightColor_); // Constructor
	~LightSource();
	inline glm::vec3 GetLightPosition() const { return position; }
	inline float GetAmbient() const { return ambient; }
	inline float GetDiffuse() const { return diffuse; }
	inline glm::vec3 GetLightColor() const { return lightColor; }

	void SetPosition(glm::vec3 position_);
	void SetLightColor(glm::vec3 lightColor_);
	void SetAmbient(float ambient_);
	void SetDiffuse(float diffuse_);

private:
	glm::vec3 position, lightColor;
	float ambient, diffuse;
};

#endif