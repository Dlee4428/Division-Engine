#include "GameObject.h"
#include "Mesh.h"
#include "../../Graphic/Shader.h"

GameObject::GameObject(Mesh *mesh_, Shader *shader_, Texture* texture_) : 
	mesh(nullptr), shader(nullptr), skybox(nullptr), texture(nullptr) {
	
	mesh = mesh_;
	shader = shader_;
	texture = texture_;
	modelMatrixID = shader->getUniformID("modelMatrix");
	normalMatrixID = shader->getUniformID("normalMatrix");

}

GameObject::GameObject(Mesh* mesh_, Shader* shader_, SkyBox* skybox_) : 
	mesh(nullptr), shader(nullptr), skybox(nullptr), texture(nullptr) {

	mesh = mesh_;
	shader = shader_;
	skybox = skybox_;
	modelMatrixID = shader->getUniformID("modelMatrix");
	normalMatrixID = shader->getUniformID("normalMatrix");
}

GameObject::~GameObject() {}

void GameObject::Update(float deltaTime_) {}

void GameObject::Render() const {
	glm::mat3 normalMatrix = glm::mat3(modelMatrix);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	if (skybox) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
	}
	
	mesh->Render();

	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}