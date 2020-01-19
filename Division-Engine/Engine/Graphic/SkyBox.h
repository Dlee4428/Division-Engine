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
		"Resources/Textures/CubeMap/posx.jpg",
		"Resources/Textures/CubeMap/negx.jpg",
		"Resources/Textures/CubeMap/posy.jpg",
		"Resources/Textures/CubeMap/negy.jpg",
		"Resources/Textures/CubeMap/posz.jpg",
		"Resources/Textures/CubeMap/negz.jpg"
	};
};
#endif // SKYBOX_H

