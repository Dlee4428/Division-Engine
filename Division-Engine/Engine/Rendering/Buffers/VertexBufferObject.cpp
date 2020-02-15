#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(unsigned int verticesNumber_) : verticesNumber(verticesNumber_)
{
	// Position of the Attribute is 0 by default
	AddAttribute(3, sizeof(float)); 
}

VertexBufferObject::~VertexBufferObject()
{
	DeleteLocalData();
}

void VertexBufferObject::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, name);
}

void VertexBufferObject::LoadGPU(bool afterGPU_)
{
	for (VertexAttribute desc : descriptionData.attributes) {
		// Grabbing sizeInBytes from glewBuffer.h
		sizeInBytes += verticesNumber * desc.elementNum * desc.elementByteSize;
	}

	// Create storage in buffer and send NULL data
	glNamedBufferStorage(name, sizeInBytes, 0, GL_DYNAMIC_STORAGE_BIT);

	int offset = 0;
	for (unsigned int i = 0; i < descriptionData.attributes.size(); ++i) {
		VertexAttribute& desc = descriptionData.attributes[i];

		// GLsizeiptr -> Non - negative binary integer size, for memory offsetsand ranges
		GLsizeiptr size = verticesNumber * desc.elementNum * desc.elementByteSize;
		
		// glNamedBufferSubData -> Updates a subset of a buffer object's data store
		glNamedBufferSubData(name, offset, size, attributeData[i]);
		offset += size;
	}

	// After GPU processed proceed to delete and clear local data
	if (afterGPU_) {
		DeleteLocalData();
	}
}

// Add All attribute from AddAtributeToVAO()
int VertexBufferObject::AddAttribute(unsigned int elementNum_, unsigned int elementByteSize_, GLenum type_, GLboolean normalized_)
{
	// Grab data from Vertex Attribute Struct
	VertexAttribute desc;
	desc.elementNum = elementNum_;
	desc.elementByteSize = elementByteSize_;
	desc.type = type_;
	desc.normalized = normalized_;
	descriptionData.attributes.push_back(desc);

	int attributeIndex = descriptionData.attributes.size() - 1;

	// Unsigned int length of 8 bits
	uint8_t* data = new uint8_t[verticesNumber * elementNum_ * elementByteSize_];
	attributeData.push_back(data);

	AddAtributeToVAO(attributeIndex);

	return attributeIndex;
}

// Add attribute to VAO single data proceed to AddAtribute()
void VertexBufferObject::AddAtributeToVAO(int attributeId_)
{
	int offset = 0;
	for (int i = 0; i < attributeId_; ++i) {
		offset += descriptionData.attributes[i].elementByteSize
			* descriptionData.attributes[i].elementNum
			* verticesNumber;
	}

	VertexAttribute& desc = descriptionData.attributes[attributeId_];
	int stride = desc.elementByteSize * desc.elementNum;

	// Attach VAO's attributes here
	vao.EnableVertexAttribute(attributeId_, name, offset, stride);
	vao.SetVertexAttribute(attributeId_, desc.elementNum, desc.type, desc.normalized, 0);
}

void VertexBufferObject::SetAttributeData(int attributeId_, const void* data_)
{
	// Reference from VertexAttribute struct
	VertexAttribute& desc = descriptionData.attributes[attributeId_];

	// Copy block of memory
	// Copies the values of num bytes from the location pointed 
	// to by source directly to the memory block pointed to by destination.
	// Param -> (destination, source, num)
	memcpy(attributeData[attributeId_], data_,
		verticesNumber * desc.elementNum * desc.elementByteSize);
}

void VertexBufferObject::SetVertexAttributeData(int attributeId_, int vertexIndex_, const void* data_)
{
	VertexAttribute& desc = descriptionData.attributes[attributeId_];
	uint8_t* offset = attributeData[attributeId_]
		+ (vertexIndex_ * desc.elementByteSize * desc.elementNum);

	memcpy(offset, data_, desc.elementByteSize * desc.elementNum);
}

void VertexBufferObject::DeleteLocalData()
{
	for (unsigned int i = 0; i < descriptionData.attributes.size(); ++i) {
		if (attributeData[i] != 0) {
			delete[] attributeData[i];
			attributeData[i] = 0;
		}
	}
}
