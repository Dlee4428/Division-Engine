#ifndef GLEWBUFFER_H
#define GLEWBUFFER

#include "GlewObject.h"

class GlewBuffer : public GlewObject {
public:
	GlewBuffer();
	~GlewBuffer();

	virtual void OnCreate();
	virtual void OnDestroy();

	inline unsigned int GetSizeInBytes() const { return sizeInBytes; }

protected:
	unsigned int sizeInBytes;
};

#endif // !GLEWBUFFER_H
