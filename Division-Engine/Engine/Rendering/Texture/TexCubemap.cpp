#include "TexCubemap.h"

TexCubemap::TexCubemap(bool yInverse_) : 
	TextureHandler(GL_TEXTURE_CUBE_MAP), yInverse(yInverse_) {

	// If enabled, cubemap textures are sampled such that when linearly sampling 
	// from the border between two adjacent faces, texels from both faces are used 
	// to generate the final sample value.When disabled, 
	// texels from only a single face are used to construct the final sample value.
	// Source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnable.xhtml

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

TexCubemap::~TexCubemap()
{
}

void TexCubemap::InitFromImageFile(const std::string& imagePath_, const ImageFormatType& imageFormat_)
{
	// It's much like std::filesystem to find path and extension
	// But this is dynamic way to do it
	for (int i = 0; i < 6; ++i) {
		std::string imagePath = "";
		std::string filePath = imagePath_.substr(0, imagePath_.length() - 4);
		std::string ext = imagePath_.substr(imagePath_.length() - 3, 3);

		int str = i;
		if (yInverse) 
		{
			if (i == 2) 
			{
				str = 3;
			}
			else if (i == 3) 
			{
				str = 2;
			}
		}
		// Append is to extends the string by appending additional characters 
		// at the end of its current value
		imagePath.append(filePath).append(std::to_string(str)).append(".").append(ext);
		images.push_back(LoadFileType(imagePath, imageFormat_));
	}

	LoadGPU();
}

void TexCubemap::LoadGPU(bool afterGPU_)
{
	// glTextureStorage2D specify the storage requirements for all levels of a
	// two - dimensional texture or one - dimensional texture array simultaneously.Once a texture is
	// specified with this command, the formatand dimensions of all levels become immutable
	// unless it is a proxy texture.The contents of the image may still be modified, however,
	// its storage requirements may not change.Such a texture is referred to as an
	// immutable - format texture.
	// Source - http://manpages.ubuntu.com/manpages/cosmic/man3/glTexStorage2D.3G.html
	glTextureStorage2D(name, mipmapLevels, images[0].format.sizedFormat, 
		images[0].width, images[0].height); //allocate space for all
	
	// From TextureHandler Bind() to active texture and bind them 
	Bind();

	for (int i = 0; i < 6; ++i) {
		ImageDataType& dataType = images[i];

		// Texturing maps a portion of a specified texture image onto each 
		// graphical primitive for which texturing is enabled.
		// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml
		// SubImageData Setters for Level 0 mipmap
		if (dataType.data != 0) {
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, dataType.width, dataType.height,
				dataType.format.baseFormat, dataType.format.type, dataType.data);
		}
		// After Load all texture clear memory
		if (afterGPU_) {
			delete[] dataType.data;
			dataType.data = 0;
		}
	}

	// If mipmaps are still remaining, generate rest mipmaps
	if (mipmapLevels > 1) {
		glGenerateTextureMipmap(name); //generate the remaining mipmaps
		glTextureParameterf(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {
		glTextureParameterf(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTextureParameterf(name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(name, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameterf(name, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameterf(name, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
