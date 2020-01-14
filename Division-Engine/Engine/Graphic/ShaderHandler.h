#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include "../../DivisionPCH.h"
#include "../Core/Debug/Debug.h"

class ShaderHandler
{
public:
	ShaderHandler(const ShaderHandler&) = delete;
	ShaderHandler(ShaderHandler&&) = delete;
	ShaderHandler& operator=(const ShaderHandler&) = delete;
	ShaderHandler& operator=(ShaderHandler&&) = delete;

	static ShaderHandler* GetInstance();
	void CreateProgram(const std::string& shaderName_,
		const std::string& vertexShaderFileName_,
		const std::string& fragmentShaderFileName_);
	static const GLuint GetShader(const std::string& shaderName_);
	void OnDestroy();
private:
	ShaderHandler();
	~ShaderHandler();

	static std::unique_ptr<ShaderHandler> shaderInstance;
	friend std::default_delete<ShaderHandler>;

	std::string ReadShader(const std::string& fileName_);
	GLuint CreateShader(GLenum shaderType_, const std::string& source_,
		const std::string& shaderName_);

	static std::map<std::string, GLuint> programs;
};
#endif

