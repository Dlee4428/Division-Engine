#ifndef TEX2DARRAY_H
#define TEX2DARRAY_H

#include "TextureHandler.h"

// FOR GL_TEXTURE_2D_ARRAY Setup
class Tex2DArray : public TextureHandler {
public:
	Tex2DArray();
	virtual ~Tex2DArray();

	virtual void Process(bool afterProcess_ = false);

private:
};

#endif // !TEX2DArray_H
