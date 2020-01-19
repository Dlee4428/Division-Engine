#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../../DivisionPCH.h"
#include "../../Graphic/Texture.h"
#include "../../Graphic/SkyBox.h"

class GameObject {
private:
	glm::mat4 modelMatrix;
	GLuint modelMatrixID;
	GLuint normalMatrixID;
	class Mesh* mesh;
	class Shader* shader;
	class Texture* texture;
	class SkyBox* skybox;
public:
	GameObject(Mesh *mesh_, Shader *shader_, Texture *texture_);
	GameObject(Mesh *mesh_, Shader *shader_, SkyBox* skybox_);
	~GameObject();
	void Render() const;
	void Update(float deltaTime_);
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const glm::mat4 &modelMatrix_) { modelMatrix = modelMatrix_; }  //glm test
	inline glm::mat4& getModelMatrix() { return modelMatrix; } //glm test
};

#endif

