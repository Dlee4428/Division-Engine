#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H

#include "../../../DivisionPCH.h"
#include "GlewBuffer.h"
#include "VertexArrayObject.h"

struct VertexAttribute {
	unsigned int elementNum;
	unsigned int elementByteSize;
	GLenum type;
	GLboolean normalized;
};

struct VBOAttributeDescription {
	std::vector<VertexAttribute> attributes;
};

// VERTEX BUFFER OBJECT
class VertexBufferObject : public GlewBuffer {
public:
	VertexBufferObject(unsigned int verticesNumber_);
	~VertexBufferObject();

	// Adding Attribute for VBO
	int AddAttribute(unsigned int elementNum_, unsigned int elementByteSize_, 
		GLenum type_ = GL_FLOAT, GLboolean normalized_ = GL_FALSE);

	// Set the All Attribute data
	void SetAttributeData(int attributeId_, const void* data_);
	
	// Atribute data for single vertex
	void SetVertexAttributeData(int attributeId_, int vertexIndex_, 
		const void* data_);

	virtual void Bind() const;
	virtual void Process(bool afterProcess_ = true);

	inline const VertexArrayObject& GetVAO() const { return vao; }
	inline unsigned int GetVerticesNumber() const { return verticesNumber; }

private:
	VBOAttributeDescription descriptionData;

	// Generic data for each attribute
	// uint8_t is the type of unsigned integer of length 8 bits
	std::vector<uint8_t*> attributeData; 

	unsigned int verticesNumber;

	VertexArrayObject vao; // VAO supports VBO's content

	void AddAtributeToVAO(int attributeId_);
	void DeleteLocalData();
};

#endif // !VERTEXBUFFEROBJECT_H
