#include "GlewBuffer.h"

GlewBuffer::GlewBuffer() : sizeInBytes(0) {
	OnCreate();
}

GlewBuffer::~GlewBuffer()
{
	OnDestroy();
}

void GlewBuffer::OnCreate()
{
	glCreateBuffers(1, &name);
}

void GlewBuffer::OnDestroy()
{
	glDeleteBuffers(1, &name);
}
