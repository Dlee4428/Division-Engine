#ifndef SHADER_H
#define SHADER_H

#include "../../../DivisionPCH.h"
#include "../../Core/Entity/Entity.h"

enum ShaderDefinition {
	VERTEX, 
	FRAGMENT, 
	TESSELLATION_CONTROL, 
	TESSELLATION_EVALUATION, 
	GEOMETRY
};


class Shader : public Entity {
public:
	Shader(const std::string& shaderPath_);
	virtual ~Shader();

	void OnDestroy();
	inline ShaderDefinition GetShaderDef() const { return shaderDef; }
	inline GLuint GetShader() const { return shader; }
private:
	ShaderDefinition shaderDef;
	GLuint shader;

	GLenum GetGLShaderDef(ShaderDefinition shaderDef_) const;

	void RefShaderDefition(const std::string& shaderPath_);
	void ReadFromFile(const std::string& filePath_, std::string& fileOut) const;
	void CheckCompilation(const GLuint shader_) const;
};


#endif // !SHADER_H
