#include "IndexBufferObject.h"

IndexBufferObject::IndexBufferObject(unsigned int indicesCount_) :indicesNumber(indicesCount_), actualPointer(0) {
	// IBO own pointer to reference indicesCount_ array
	this->indices = new unsigned int[indicesCount_];
}

IndexBufferObject::~IndexBufferObject() {
	DeleteLocalData();
}

void IndexBufferObject::SetIndices(GLuint* indices_) {
	// Copy block of memory 
	// Copies the value of num bytes from the location pointed to by source directly
	// to by destination.
	// Param = (destination, source, num)
	memcpy(this->indices, indices_, indicesNumber * sizeof(unsigned int));
}

void IndexBufferObject::AddIndex(GLuint index_)
{
	// Adds Index
	// Param actualPointer = unsigned int
	indices[actualPointer++] = index_;
}

void IndexBufferObject::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, name);
}

void IndexBufferObject::LoadGPU(bool afterGPU_)
{
	// Using sizeof to find out number of elements in a indices array
	sizeInBytes = indicesNumber * sizeof(unsigned int);
	glNamedBufferStorage(name, sizeInBytes, indices, 0);

	if (afterGPU_) {
		DeleteLocalData();
	}
}

void IndexBufferObject::DeleteLocalData() {
	if (this->indices != 0) {
		delete[] this->indices;
		this->indices = 0;
	}
}
