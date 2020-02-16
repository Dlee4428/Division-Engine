#ifndef SKYBOX_H
#define SKYBOX_H

#include "../../DivisionPCH.h"

#include "../Rendering/SceneObject.h"
#include "../Rendering/Shader/Shader.h"
#include "../Rendering/Shader/ShaderProgram.h"
#include "../Rendering/Texture/TexCubemap.h"
#include "../Rendering/3D/Mesh.h"
#include "../Rendering/3D/MaterialHandler.h"

class Skybox : public SceneObject {
public:
	Skybox(const std::string& texLocation_);
	~Skybox();

	virtual void OnCreate();
	virtual void Render(int passIDref_);
	virtual void Update(double deltaTime_);

private:
	std::string texLocation;
};

#endif // !SKYBOX_H
