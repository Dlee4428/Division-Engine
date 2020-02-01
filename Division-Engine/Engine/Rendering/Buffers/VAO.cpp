#include "VAO.h"


VAO::VAO() {
	OnCreate();
}

VAO::~VAO() {
	OnDestroy();
}

void VAO::OnCreate() {
	glCreateVertexArrays(1, &name);
}

void VAO::OnDestroy() {
	glDeleteVertexArrays(1, &name);
}

void VAO::Bind() const {
	glBindVertexArray(name);
}

void VAO::EnableVertexAttribute(GLuint indexAttribute_, GLuint vbo_, 
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

void VAO::SetVertexAttribute(GLuint indexAttribute_, GLint size_, 
	GLenum type_, GLboolean normalized_, GLuint relativeOffset_) {
	// Specify the format of the attributes 
	// PRE 4.5 = glVertexAttribFormat()
	glVertexArrayAttribFormat(name, indexAttribute_, size_, type_, normalized_, relativeOffset_);
}