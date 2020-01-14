#include "Mesh.h"

Mesh::Mesh(SubMesh subMesh_, GLuint shaderProgram_) : VAO(0), VBO(0),
shaderProgram(0), modelLoc(0), viewLoc(0), projeLoc(0), diffuseMapLoc(0), viewPos(0), lightPos(0), ambient(0), diffuse(0), lightColor(0), subMesh(SubMesh()) {
	shaderProgram = shaderProgram_;
	subMesh = subMesh_;
	GenerateBuffers();
}

Mesh::~Mesh() {
	OnDestroy();
}

void Mesh::Render(Camera* camera_, std::vector<glm::mat4>& instances_) {
	glUniform1i(diffuseMapLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseMap);
	glEnable(GL_DEPTH_TEST);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projeLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));

	glUniform3fv(viewPos, 1, glm::value_ptr(camera_->GetPosition())); //2019-10-15
	glUniform3f(lightPos,
		camera_->GetLightSources()[0]->GetLightPosition().x,
		camera_->GetLightSources()[0]->GetLightPosition().y,
		camera_->GetLightSources()[0]->GetLightPosition().z);
	glUniform1f(ambient, camera_->GetLightSources()[0]->GetAmbient());
	glUniform1f(diffuse, camera_->GetLightSources()[0]->GetDiffuse());
	glUniform3f(lightColor,
		camera_->GetLightSources()[0]->GetLightColor().x,
		camera_->GetLightSources()[0]->GetLightColor().y,
		camera_->GetLightSources()[0]->GetLightColor().z);

	// Homework to setup material
	glUniform1f(shininessLoc, subMesh.material.shininess);
	glUniform1f(transparencyLoc, subMesh.material.transparency);
	glUniform3f(ambientLoc,
		subMesh.material.ambient.x,
		subMesh.material.ambient.y,
		subMesh.material.ambient.z);
	glUniform3f(diffuseLoc,
		subMesh.material.diffuse.x,
		subMesh.material.diffuse.y,
		subMesh.material.diffuse.z);
	glUniform3f(specLoc,
		subMesh.material.specular.x,
		subMesh.material.specular.y,
		subMesh.material.specular.z);

	glBindVertexArray(VAO);
	for (int i = 0; i < instances_.size(); i++) {
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(instances_[i]));
		glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());

	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Mesh::OnDestroy() {
	glDeleteVertexArrays(1, &VAO); // & address the non pointer
	glDeleteBuffers(1, &VBO);

	subMesh.vertexList.clear();
}

void Mesh::GenerateBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, subMesh.vertexList.size() * sizeof(Vertex), &subMesh.vertexList[0], GL_STATIC_DRAW);

	// POSITION
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// NORMAL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	// TEXTURE COORDINATES
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

	// COLOURS
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, colour));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	modelLoc = glGetUniformLocation(shaderProgram, "model");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projeLoc = glGetUniformLocation(shaderProgram, "proj");

	//2019-10-15
	viewPos = glGetUniformLocation(shaderProgram, "viewPosition");
	lightPos = glGetUniformLocation(shaderProgram, "light.lightPos");
	ambient = glGetUniformLocation(shaderProgram, "light.ambient");
	diffuse = glGetUniformLocation(shaderProgram, "light.diffuse");
	lightColor = glGetUniformLocation(shaderProgram, "light.lightColor");

	diffuseMapLoc = glGetUniformLocation(shaderProgram, "material.diffuseMap");
	shininessLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	transparencyLoc = glGetUniformLocation(shaderProgram, "material.transparency");
	ambientLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	diffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	specLoc = glGetUniformLocation(shaderProgram, "material.specular");
}