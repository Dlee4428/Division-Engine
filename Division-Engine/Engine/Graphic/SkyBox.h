#ifndef SKYBOX_H
#define SKYBOX_H

#include "../../DivisionPCH.h"
#include "Shader.h"

class SkyBox
{
private:
	int mode;
	GLuint textureID; // GLuint is the unsigned int typedef from glew
	SDL_Surface* textureSurface;

	class Mesh* meshPtr;
	class Shader* shaderPtr;
	SkyBox* skyboxPtr;
	class GameObject* skyObject;
public:
	SkyBox();
	~SkyBox();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;
	inline GLuint getTextureID() const { return textureID; }
	inline Shader* getShader() const { return shaderPtr; }
	bool LoadSkyBox(const std::vector<std::string> faces);

	// SkyBox OnCreate()
	std::vector<std::string> faces
	{
		"./Resources/Textures/DaySky/posx.jpg",
		"./Resources/Textures/DaySky/negx.jpg",
		"./Resources/Textures/DaySky/posy.jpg",
		"./Resources/Textures/DaySky/negy.jpg",
		"./Resources/Textures/DaySky/posz.jpg",
		"./Resources/Textures/DaySky/negz.jpg"
	};
};
#endif // SKYBOX_H

