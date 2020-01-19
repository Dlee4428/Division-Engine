#ifndef MESH_H
#define MESH_H

#include "../../../DivisionPCH.h"

class Mesh {
public:
	GLenum drawmode;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvCoords;
	std::vector<GLuint> indices;

	Mesh(GLenum drawmode_, std::vector<glm::vec3>&, std::vector<glm::vec3>&, std::vector<glm::vec2>&);
	~Mesh();
	void Render() const;
private:
	GLuint vao, vbo;
	void setupMesh();
};

#endif
