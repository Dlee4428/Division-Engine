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


TextureHandler::TextureHandler(GLenum textureType_)
{
}

TextureHandler::~TextureHandler()
{
}

void TextureHandler::OnCreate()
{
}

void TextureHandler::OnDestroy()
{
}

void TextureHandler::Bind() const
{
}

void TextureHandler::Bind(int unit_) const
{
}

void TextureHandler::ModifyTextureParam(GLenum param_, GLfloat value_)
{
}

const void* TextureHandler::GetTexelImage(const ImageDataType& imageDataType_, const int x_, const int y_, const int z_)
{
	return nullptr;
}

void TextureHandler::SetTexelImage(int imageIndex_, const int x_, const int y_, const int z_, const void* value_)
{
}

void TextureHandler::InitFromImageFile(const std::string& imagePath_, const ImageFormatType& imageFormat_)
{
}

void TextureHandler::InitFromImageFiles(const std::vector<std::string>& imagePaths_, const ImageFormatType& imageFormat_)
{
}

void TextureHandler::InitFromImageData(uint32_t width_, uint32_t height_, const uint8_t* imageData, const ImageFormatType& imageFormat_)
{
}

const ImageDataType TextureHandler::LoadFileType(const std::string& imagePath_, const ImageFormatType& imageFormat_)
{
	return;
}
