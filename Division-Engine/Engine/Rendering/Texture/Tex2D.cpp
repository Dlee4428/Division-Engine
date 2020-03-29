#include "Tex2D.h"

Tex2D::Tex2D() : TextureHandler(GL_TEXTURE_2D)
{
}

Tex2D::~Tex2D()
{
}

void Tex2D::Process(bool afterProcess_)
{
	// Accessing Protected variable from ImageDataType vector
	ImageDataType& dataType = images[0];

	// glTextureStorage2D specify the storage requirements for all levels of a
	// two - dimensional texture or one - dimensional texture array simultaneously.Once a texture is
	// specified with this command, the formatand dimensions of all levels become immutable
	// unless it is a proxy texture.The contents of the image may still be modified, however,
	// its storage requirements may not change.Such a texture is referred to as an
	// immutable - format texture.
	// Source - http://manpages.ubuntu.com/manpages/cosmic/man3/glTexStorage2D.3G.html
	glTextureStorage2D(name, mipmapLevels, 
		dataType.format.sizedFormat, dataType.width, dataType.height);

	if (dataType.data != 0) {

		// Texturing maps a portion of a specified texture image onto each 
		// graphical primitive for which texturing is enabled.
		// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexSubImage2D.xhtml
		// SubImageData Setters for Level 0 mipmap
		glTextureSubImage2D(name, 0, 0, 0, dataType.width, dataType.height,
			dataType.format.baseFormat, dataType.format.type, dataType.data);
	}

	// After Load all texture clear memory
	if (afterProcess_) {
		delete[] dataType.data;
		dataType.data = 0;
	}

	// If mipmaps are still remaining, generate rest mipmaps
	if (mipmapLevels > 1) {
		glGenerateTextureMipmap(name);
		glTextureParameterf(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {
		glTextureParameterf(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTextureParameterf(name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameterf(name, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameterf(name, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
