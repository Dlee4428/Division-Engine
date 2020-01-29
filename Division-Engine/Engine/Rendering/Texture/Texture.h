#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../DivisionPCH.h"
#include "../../Core/Entity/Entity.h"
#include "../Buffers/GlewObject.h"

// Determines Types of Image formats
struct ImageFormat {
	GLenum sizedFormat, baseFormat, type;
	uint32_t numberChannels, bytesPerChannel;

	const static ImageFormat IMAGE_FORMAT_SRGBA8;
	const static ImageFormat IMAGE_FORMAT_SRGB8;

	const static ImageFormat IMAGE_FORMAT_SBGR8;

	const static ImageFormat IMAGE_FORMAT_RGBA8_LINEAR;
	const static ImageFormat IMAGE_FORMAT_RGB8_LINEAR;

	const static ImageFormat IMAGE_FORMAT_R8;
	const static ImageFormat IMAGE_FORMAT_R16;

	const static ImageFormat IMAGE_FORMAT_DEPTH_32F;

	ImageFormat(GLenum sizedFormat_, GLenum baseFormat_, GLenum type_, uint32_t numberChannels_, uint32_t bytesPerChannel_) :
		sizedFormat(sizedFormat_), baseFormat(baseFormat_), type(type_), numberChannels(numberChannels_), bytesPerChannel(bytesPerChannel_) {
	}

	ImageFormat(){}
};

// Image Data holder
struct ImageData {
	ImageFormat format;
	uint32_t width, height;
	uint8_t* data;
	uint32_t sizeInBytes;
};


class Texture : public GlewObject, public Entity {
public:

private:

};

#endif // !TEXTURE_H
