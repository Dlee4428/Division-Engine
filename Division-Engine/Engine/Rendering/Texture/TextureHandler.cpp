#include "TextureHandler.h"

// GLOBAL SETTERS FOR IMAGE FORMAT TYPE REFERENCE
const ImageFormatType ImageFormatType::IMAGE_FORMAT_SRGBA8 = ImageFormatType(GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 1);
const ImageFormatType ImageFormatType::IMAGE_FORMAT_SRGB8 = ImageFormatType(GL_SRGB8, GL_RGB, GL_UNSIGNED_BYTE, 3, 1);
const ImageFormatType ImageFormatType::IMAGE_FORMAT_SBGR8 = ImageFormatType(GL_SRGB8, GL_BGR, GL_UNSIGNED_BYTE, 3, 1);
const ImageFormatType ImageFormatType::IMAGE_FORMAT_RGBA8_LINEAR = ImageFormatType(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 1);
const ImageFormatType ImageFormatType::IMAGE_FORMAT_RGB8_LINEAR = ImageFormatType(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 3, 1);
const ImageFormatType ImageFormatType::IMAGE_FORMAT_R8 = ImageFormatType(GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1, 1);
const ImageFormatType ImageFormatType::IMAGE_FORMAT_R16 = ImageFormatType(GL_R16, GL_RED, GL_UNSIGNED_SHORT, 1, 2);
const ImageFormatType ImageFormatType::IMAGE_FORMAT_DEPTH_32F = ImageFormatType(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, 1, 4);


TextureHandler::TextureHandler(GLenum textureType_) : type(textureType_), mipmapLevels(8) {
	OnCreate();
}

TextureHandler::~TextureHandler()
{
	OnDestroy();

}

void TextureHandler::OnCreate()
{
	glCreateTextures(type, 1, &name);
}

void TextureHandler::OnDestroy()
{
	// Clear out Textures and image memory data 
	glDeleteTextures(1, &name);

	for (unsigned int i = 0; i < images.size(); ++i) {
		if (images[i].data != 0) {
			delete[] images[i].data;
		}
	}
}

void TextureHandler::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(type, name);
}

void TextureHandler::Bind(int unit_) const
{
	glBindTextureUnit(unit_, name);
}

void TextureHandler::ModifyTextureParam(GLenum param_, GLfloat value_)
{
	// TEXTURE WRAP and VALUE parameter for terrain
	glTextureParameterf(name, param_, value_);
}

void TextureHandler::InitFromImageFile(const std::string& imagePath_, const ImageFormatType& imageFormat_)
{
	// Push single file element into vector
	// With the type of DevIl image LoadPath
	images.push_back(LoadFileType(imagePath_, imageFormat_));
	LoadGPU();
}

void TextureHandler::InitFromImageFiles(const std::vector<std::string>& imagePaths_, const ImageFormatType& imageFormat_)
{
	// Push multiple file elements into vector
	for (auto p : imagePaths_) {
		images.push_back(LoadFileType(p, imageFormat_));
	}
	LoadGPU();
}

void TextureHandler::InitFromImageData(uint32_t width_, uint32_t height_, const uint8_t* imageData_, const ImageFormatType& imageFormat_)
{
	// For the FBO function, init depthTexture Data for Terrain
	ImageDataType imageData;
	imageData.format = imageFormat_;

	imageData.width = width_;
	imageData.height = height_;
	imageData.sizeInBytes = width_ * height_ * imageFormat_.numberChannels * imageFormat_.bytesPerChannel;

	if (imageData_ != 0) {
		imageData.data = new uint8_t[imageData.sizeInBytes];
		memcpy(imageData.data, imageData_, imageData.sizeInBytes);
	}
	else
		imageData.data = 0;

	images.push_back(imageData);

	LoadGPU();
}

const ImageDataType TextureHandler::LoadFileType(const std::string& imagePath_, const ImageFormatType& imageFormat_)
{
	// DevIl Image Loader
	ilInit();

	ILuint imageIl;
	ilGenImages(1, &imageIl);
	ilBindImage(imageIl);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ImageDataType imageData;

	ILboolean result = ilLoadImage(imagePath_.c_str());

	if (result) {
		imageData.format = imageFormat_;
		imageData.width = ilGetInteger(IL_IMAGE_WIDTH);
		imageData.height = ilGetInteger(IL_IMAGE_HEIGHT);
		
		imageData.sizeInBytes = 
			imageData.width * 
			imageData.height * 
			imageData.format.numberChannels * 
			imageData.format.bytesPerChannel;

		imageData.data = new uint8_t[imageData.sizeInBytes];
		memcpy(imageData.data, ilGetData(), imageData.sizeInBytes);
	}
	else {
		Debug::FatalError("Cannot access image file " + imagePath_, __FILE__, __LINE__);
	}

	ilDeleteImages(1, &imageIl);

	return imageData;
}

const void* TextureHandler::GetTexelImage(const ImageDataType& imageDataType_, const int x_, const int y_, const int z_)
{
	// Get the Texel sizes from images format bytes per channel and number of channels of data
	uint32_t sizeOfTexel = imageDataType_.format.bytesPerChannel * imageDataType_.format.numberChannels;
	uint8_t* texelPtr = imageDataType_.data + (y_ * sizeOfTexel * imageDataType_.width) + (x_ * sizeOfTexel);
	return texelPtr;
}

void TextureHandler::SetTexelImage(int imageIndex_, const int x_, const int y_, const int z_, const void* value_)
{
	// Set the Texel sizes from images format bytes per channel and number of channels of data
	const ImageDataType& imageData = images[imageIndex_];
	uint32_t texelSize = imageData.format.bytesPerChannel * imageData.format.numberChannels;
	uint8_t* ptr = imageData.data + (y_ * texelSize * imageData.width) + (x_ * texelSize);

	// Dist, source, size
	memcpy(ptr, value_, texelSize);
}
