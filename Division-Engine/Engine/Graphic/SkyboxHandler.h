#ifndef SKYBOXHANDLER_H
#define SKYBOXHANDLER_H

#include "../../DivisionPCH.h"
#include "../Core/Debug/Debug.h"

struct Skybox {
	GLuint textureID = 0;
	float width = 0.0f;
	float height = 0.0f;
};

class SkyboxHandler
{
public:
	//Singleton design pattern
	SkyboxHandler(const SkyboxHandler&) = delete;
	SkyboxHandler(SkyboxHandler&&) = delete;
	SkyboxHandler& operator=(const SkyboxHandler&) = delete;
	SkyboxHandler& operator=(SkyboxHandler&&) = delete;

	static SkyboxHandler* GetInstance();
	void CreateCubeMap(const std::string& skyboxName_, const std::string& skyboxFileName_);
	static const GLuint GetTexture(const std::string& skyboxName_);
	static const Skybox* GetTextureData(const std::string& skyboxName_);
	void OnDestroy();
private:
	SkyboxHandler();
	~SkyboxHandler();

	static std::unique_ptr<SkyboxHandler> skyboxInstance;
	friend std::default_delete<SkyboxHandler>;
	static std::map<std::string, Skybox*> skyboxes;
};

#endif // 