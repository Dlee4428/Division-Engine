#include "LightSource.h"

// 2019-10-15
LightSource::LightSource(glm::vec3 position_, float ambient_, float diffuse_, glm::vec3 lightColor_) :
	position(glm::vec3()), ambient(0), diffuse(0), lightColor(glm::vec3()) {
	position = position_;
	ambient = ambient_;
	diffuse = diffuse_;
	lightColor = lightColor_;
}

LightSource::~LightSource()
{
}

void LightSource::SetPosition(glm::vec3 position_)
{
	position = position_;
}

void LightSource::SetAmbient(float ambient_)
{
	ambient = ambient_;
}

void LightSource::SetDiffuse(float diffuse_)
{
	diffuse = diffuse_;
}

void LightSource::SetLightColor(glm::vec3 lightColor_)
{
	lightColor = lightColor_;
}