#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"

class ShaderProgram : public Entity {
public:
	ShaderProgram(const std::vector<Shader*>& shader_);
	virtual ~ShaderProgram();

	void OnDestroy();
	void BindShader() const;
private:
	GLuint program;
	
	void LinkStatus(const GLuint program_) const;
};

#endif // !SHADERPROGRAM_H
