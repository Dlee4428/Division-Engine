#ifndef WATERFRAMEBUFFER_H
#define WATERFRAMEBUFFER_H

#include "GlewObject.h"
#include "../Texture/TextureHandler.h"

class WaterFrameBuffer : public GlewObject {
public:
	WaterFrameBuffer();
	~WaterFrameBuffer();

	virtual void OnCreate();
	virtual void OnDestroy();



private:

};

#endif // !WATERFRAMEBUFFER_H
