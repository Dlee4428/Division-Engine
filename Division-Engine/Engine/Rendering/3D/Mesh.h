#ifndef MESH_H
#define MESH_H

#include "../../../DivisionPCH.h"
#include "../../Camera/Camera.h"
#include "../../Graphic/MaterialHandler.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 colour;
};

struct SubMesh {
	std::vector<Vertex> vertexList;
	std::vector<int> meshIndices;
	Material material;
};

class Mesh
{
public:
	Mesh(SubMesh subMesh, GLuint shaderProgram_); // By referencing vector as pointer, it saves the memory allocation space
	~Mesh();
	void Render(Camera* camera_, std::vector<glm::mat4>& instances_);
	void OnDestroy();
private:
	void GenerateBuffers();
	GLuint VAO, VBO;
	GLuint shaderProgram;
	GLuint modelLoc, viewLoc, projeLoc;
	GLuint viewPos, lightPos, ambient, diffuse, lightColor;
	GLuint diffuseMapLoc, shininessLoc, transparencyLoc, ambientLoc, diffuseLoc, specLoc;
	SubMesh subMesh;
};
#endif // !1

