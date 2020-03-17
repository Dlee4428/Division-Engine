#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include "GlewObject.h"

// VERTEX ARRAY OBJECT FOR MESH
class VertexArrayObject : public GlewObject {
public:
	VertexArrayObject();
	~VertexArrayObject();

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Bind() const;
	virtual void LoadGPU(bool afterGPU_ = true) {};

	void EnableVertexAttribute(GLuint indexAttribute_, GLuint vbo_, GLintptr vboOffset_, GLsizei vboStride_);
	void SetVertexAttribute(GLuint indexAttribute_, GLint size_, GLenum type_, GLboolean normalized_, GLuint relativeOffset_);
};

#endif // !VERTEXARRAYOBJECT_H
