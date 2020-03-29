#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include "../../../DivisionPCH.h"
#include "../../Core/Entity/Entity.h"
#include "../Buffers/GlewObject.h"

// Determines Types of Image formats
// Setups for DevIl image opensource
struct ImageFormatType {
	GLenum sizedFormat, baseFormat, type;
	uint32_t numberChannels, bytesPerChannel;

	const static ImageFormatType IMAGE_FORMAT_SRGBA8;
	const static ImageFormatType IMAGE_FORMAT_SRGB8;
	const static ImageFormatType IMAGE_FORMAT_SBGR8;
	const static ImageFormatType IMAGE_FORMAT_RGBA8_LINEAR;
	const static ImageFormatType IMAGE_FORMAT_RGB8_LINEAR;
	const static ImageFormatType IMAGE_FORMAT_R8;
	const static ImageFormatType IMAGE_FORMAT_R16;
	const static ImageFormatType IMAGE_FORMAT_DEPTH_32F;

	ImageFormatType(GLenum sizedFormat_, GLenum baseFormat_, GLenum type_, uint32_t numberChannels_, uint32_t bytesPerChannel_) :
		sizedFormat(sizedFormat_), baseFormat(baseFormat_), type(type_), numberChannels(numberChannels_), bytesPerChannel(bytesPerChannel_) {
	}

	ImageFormatType() {}
};

// Image Data holder
struct ImageDataType {
	ImageFormatType format;
	uint32_t width, height;
	uint8_t* data;
	uint32_t sizeInBytes;
};


class TextureHandler : public GlewObject, public Entity {
public:
	TextureHandler(GLenum textureType_);
	virtual ~TextureHandler();

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Bind() const;
	virtual void Process(bool afterProcess_ = false) = 0;

	void Bind(int unit_) const;
	void ModifyTextureParam(GLenum param_, GLfloat value_);

	// For Setting Terrains into single texture such as heightMap and Normal mapping
	virtual void InitFromImageFile(const std::string& imagePath_, 
		const ImageFormatType& imageFormat_ = ImageFormatType::IMAGE_FORMAT_SRGB8);
	
	// For Setting Terrains texture binding into array
	virtual void InitFromImageFiles(const std::vector<std::string>& imagePaths_, 
		const ImageFormatType& imageFormat_ = ImageFormatType::IMAGE_FORMAT_SRGB8);

	// For Texel Parameter width, height, and Image format Depth
	void InitFromImageData(uint32_t width_, uint32_t height_, const uint8_t* imageData_, 
		const ImageFormatType& imageFormat_ = ImageFormatType::IMAGE_FORMAT_SRGB8);

	// For Mipmap Level settings
	// Usage for Level of Detail (LOD)
	// Improve Image quality. Rendering from large textures where only small, 
	// discontiguous subsets of texels are used can easily produce moiré patterns
	// Speeding up rendering times, either by reducing the number of texel 
	// sampled to render each pixel, or increasing the memory locality of the samples taken
	// Mostly Reducing stress on the GPU or CPU
	inline void SetMipmapLevels(int mipmapLevels_) { mipmapLevels = mipmapLevels_; }


	inline const ImageDataType& GetImageData(int idx_) { return images[idx_]; }

protected:
	GLenum type;
	std::vector<ImageDataType> images;
	int mipmapLevels;

	const ImageDataType LoadFileType(const std::string& imagePath_, 
		const ImageFormatType& imageFormat_);
};

#endif // !TEXTUREHANDLER_H
