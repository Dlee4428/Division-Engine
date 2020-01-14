#ifndef TEXTUREHANDELER_H
#define TEXTUREHANDELER_H

#include "../../DivisionPCH.h"
#include "../Core/Debug/Debug.h"


struct Texture {
	GLuint textureID = 0;
	float width = 0.0f;
	float height = 0.0f;
};

class TextureHandler
{
public:
	//Singleton design pattern
	TextureHandler(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator=(const TextureHandler&) = delete;
	TextureHandler& operator=(TextureHandler&&) = delete;

	static TextureHandler* GetInstance();
	void CreateTexture2D(const std::string& textureName_, const std::string& textureFileName_);
	static const GLuint GetTexture(const std::string& textureName_);
	static const Texture* GetTextureData(const std::string& textureName_);
	void OnDestroy();
private:
	TextureHandler();
	~TextureHandler();

	static std::unique_ptr<TextureHandler> textureInstance;
	friend std::default_delete<TextureHandler>;
	static std::map<std::string, Texture*> textures;
};


#endif