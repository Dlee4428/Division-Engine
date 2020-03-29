#include "Tex2DArray.h"

Tex2DArray::Tex2DArray() : TextureHandler(GL_TEXTURE_2D_ARRAY)
{

}

Tex2DArray::~Tex2DArray()
{
}

void Tex2DArray::Process(bool afterProcess_)
{
	//////////////////////////////////////////////////////////////////////////
	// glTextureStorage3D
	// Simultaneously specify storage for all levels of a three-dimensional, 
	// Two-dimensional array or cube-map array texture
	// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexStorage3D.xhtml
	glTextureStorage3D(name, mipmapLevels, images[0].format.sizedFormat, 
		images[0].width, images[0].height, images.size());

	for (unsigned int i = 0; i < images.size(); ++i) {
		ImageDataType& dataType = images[i];

		// Specify a Three dimensional texture subimage
		// Texturing maps a portion of a specified texture image onto each graphical primitive for which texturing is enabled.
		// Source - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexSubImage3D.xhtml
		if (dataType.data != 0) {
			glTextureSubImage3D(name, 0, 0, 0, i, dataType.width, dataType.height, 1,
				dataType.format.baseFormat, dataType.format.type, dataType.data);
		}

		if (afterProcess_) {
			delete[] dataType.data;
			dataType.data = 0;
		}
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
