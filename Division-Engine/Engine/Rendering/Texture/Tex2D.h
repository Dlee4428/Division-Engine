#ifndef TEX2D_H
#define TEX2D_H

#include "TextureHandler.h"

// FOR GL_TEXTURE_2D Setup
class Tex2D : public TextureHandler {
public:
	Tex2D();
	virtual ~Tex2D();

	virtual void LoadGPU(bool afterGPU_ = false);

private:
};

#endif // !TEX2D_H
