#ifndef SKYBOX_H
#define SKYBOX_H

#include "../../DivisionPCH.h"

#include "../Rendering/GameObject.h"
#include "../Rendering/Shader/Shader.h"
#include "../Rendering/Shader/ShaderProgram.h"
#include "../Rendering/Texture/TexCubemap.h"
#include "../Rendering/3D/Mesh.h"
#include "../Rendering/3D/MaterialHandler.h"

class Skybox : public GameObject {
public:
	Skybox(const std::string& texLoc_);
	~Skybox();

	virtual void OnCreate();
	virtual void Render(int objectID_);
	virtual void Update(double deltaTime_);
	
	void SkyReflection();
	void SkyRefraction();

private:
	std::string texLocation;

	// For Frustum Top view
	glm::mat4 topViewMatrix;

	int width, height;

};

#endif // !SKYBOX_H
