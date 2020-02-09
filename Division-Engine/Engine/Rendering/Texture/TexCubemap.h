#ifndef TEXCUBEMAP_H
#define TEXCUBEMAP_H

#include "TextureHandler.h"

// FOR GL_TEXTURE_CUBEMAP Setup
class TexCubemap : public TextureHandler {
public:
	TexCubemap(bool yInverse_ = false);
	virtual ~TexCubemap();

	virtual void InitFromImageFile(const std::string& imagePath_,
		const ImageFormatType& imageFormat_ = ImageFormatType::IMAGE_FORMAT_SRGB8);
	virtual void LoadGPU(bool afterGPU_ = false);

private:
	bool yInverse;
};

#endif // !TEXCUBEMAP_H
