#include "VertexArrayObject.h"


VertexArrayObject::VertexArrayObject() {
	OnCreate();
}

VertexArrayObject::~VertexArrayObject() {
	OnDestroy();
}

void VertexArrayObject::OnCreate() {
	glCreateVertexArrays(1, &name);
}

void VertexArrayObject::OnDestroy() {
	glDeleteVertexArrays(1, &name);
}

void VertexArrayObject::Bind() const {
	glBindVertexArray(name);
}

void VertexArrayObject::EnableVertexAttribute(GLuint indexAttribute_, GLuint vbo_,
	GLintptr vboOffset_, GLsizei vboStride_) {
	// Associate vertex attributes to buffer bindings
	// glVertexAttribBinding()
	// AttributeIndex to the binding id with the same name
	glVertexArrayAttribBinding(name, indexAttribute_, indexAttribute_);

	// Associate VAO buffer bindings to the correct VBOs and with correct offsets
	// Before 4.5 = glBindVertexBuffer()
	glVertexArrayVertexBuffer(name, indexAttribute_, vbo_, vboOffset_, vboStride_);

	// Before 4.5 = glEnableVertexAttribArray()
	glEnableVertexArrayAttrib(name, indexAttribute_);
}

void VertexArrayObject::SetVertexAttribute(GLuint indexAttribute_, GLint size_,
	GLenum type_, GLboolean normalized_, GLuint relativeOffset_) {
	// Specify the format of the attributes 
	// PRE 4.5 = glVertexAttribFormat()
	glVertexArrayAttribFormat(name, indexAttribute_, size_, type_, normalized_, relativeOffset_);
}