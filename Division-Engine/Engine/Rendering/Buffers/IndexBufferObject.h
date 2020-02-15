#ifndef INDEXBUFFEROBJECT_H
#define INDEXBUFFEROBJECT_H

#include "GlewBuffer.h"

// Index Buffer Object Class using GlewBuffer
class IndexBufferObject : public GlewBuffer {
public:
	IndexBufferObject(unsigned int indicesCount_);
	~IndexBufferObject();

	void SetIndices(GLuint* indices_);
	void AddIndex(GLuint index_);

	virtual void Bind() const;
	virtual void LoadGPU(bool afterGPU_ = true);

	inline unsigned int GetIndicesNumber() const { return indicesNumber; }

private:
	GLuint* indices;

	unsigned int indicesNumber;
	unsigned int actualPointer;

	void DeleteLocalData();
};


#endif // !IndexBufferObject_H
