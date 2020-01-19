#include "Mesh.h"

Mesh::Mesh(GLenum drawmode_, std::vector<glm::vec3> &verticies_, std::vector<glm::vec3> &normals_, std::vector<glm::vec2> &uvCoords_) {
	drawmode = drawmode_;
	this->vertices = verticies_;
	this->normals = normals_;
	this->uvCoords = uvCoords_;
	this->setupMesh(); // GenerateBuffers
}

Mesh::~Mesh(){
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	
}

void Mesh::setupMesh() {
#define VERTEX_LENGTH 	(vertices.size() * (sizeof(glm::vec3)))
#define NORMAL_LENGTH 	(normals.size() * (sizeof(glm::vec3)))
#define TEXCOORD_LENGTH (uvCoords.size() * (sizeof(glm::vec2)))
	const int verticiesID = 0;
	const int normalsID = 1;
	const int uvCoordsID = 2;

	/// create and bind the VOA
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	/// Create and initialize vertex buffer object VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH + TEXCOORD_LENGTH, NULL, GL_STATIC_DRAW);

	/// assigns the addr of "points" to be the beginning of the array buffer "sizeof(points)" in length
	glBufferSubData(GL_ARRAY_BUFFER, 0, VERTEX_LENGTH, &vertices[0]);
	/// assigns the addr of "normals" to be "sizeof(points)" offset from the beginning and "sizeof(normals)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH, NORMAL_LENGTH, &normals[0]);
	/// assigns the addr of "texCoords" to be "sizeof(points) + sizeof(normals)" offset from the beginning and "sizeof(texCoords)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH, TEXCOORD_LENGTH, &uvCoords[0]);

	glEnableVertexAttribArray(verticiesID);
	glVertexAttribPointer(verticiesID, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(normalsID);
	glVertexAttribPointer(normalsID, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH));
	glEnableVertexAttribArray(uvCoordsID);
	glVertexAttribPointer(uvCoordsID, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH + NORMAL_LENGTH));


#undef VERTEX_LENGTH
#undef NORMAL_LENGTH
#undef TEXCOORD_LENGTH
}


void Mesh::Render() const {
	glBindVertexArray(vao);
	glDrawArrays(drawmode, 0, vertices.size());
	glBindVertexArray(0); // Disable the VAO
}