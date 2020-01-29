#ifndef IBO_H
#define IBO_H

#include "GlewBuffer.h"

// Index Buffer Object Class using GlewBuffer
class IBO : public GlewBuffer {
public:
	IBO(unsigned int indicesCount_);
	~IBO();

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


#endif // !IBO_H
