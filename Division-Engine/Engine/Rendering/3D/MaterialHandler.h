#ifndef MATERIALHANDLER_H
#define MATERIALHANDLER_H

#include "../Texture/TextureHandler.h" 
#include "../../Core/Entity/Entity.h"
#include "../Shader/ShaderProgram.h"

class MaterialHandler : public Entity{
public:
	MaterialHandler();
	virtual ~MaterialHandler();

	void SetTexture(unsigned int idx_, TextureHandler* texHandler_);
	void SetShaderProgram(unsigned int idx_, ShaderProgram* sProgram_);

	inline ShaderProgram* GetShaderProgram(int shader_) const { return sProgramVector[shader_]; }
	inline TextureHandler* GetTextureHandler(int texture_) const { return texVector[texture_]; }

	inline void SetActiveShader (int shaderActive_) { shaderActive = shaderActive_; }
	inline int GetTextureSize() { return texVector.size(); }

	void Bind() const;
	void BindShader() const;
	void BindTexture() const;

private:
	int shaderActive;

	std::vector<TextureHandler*> texVector;
	std::vector<ShaderProgram*> sProgramVector;
};

#endif // !MATERIALHANDLER_H
