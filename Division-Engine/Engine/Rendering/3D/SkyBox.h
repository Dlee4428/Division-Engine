#ifndef SKYBOX_H
#define SKYBOX_H

#include "../../../DivisionPCH.h"
#include "../../Camera/Camera.h"

class SkyBox {
public:
	SkyBox(GLuint shaderProgram_);
	~SkyBox();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render(Camera* camera_) const;
	unsigned int LoadCubemap(std::vector<std::string> faces);
private:
	std::vector<std::string> faces
	{
		"./Resources/Textures/CubeMap/posx.jpg",
		"./Resources/Textures/CubeMap/negx.jpg",
		"./Resources/Textures/CubeMap/posy.jpg",
		"./Resources/Textures/CubeMap/negy.jpg",
		"./Resources/Textures/CubeMap/posz.jpg",
		"./Resources/Textures/CubeMap/negz.jpg"
	};
	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
	unsigned int textureID;
	GLuint modelLoc, viewLoc, projeLoc;
	GLuint shaderProgram;
};
#endif // !SKYBOX_H
