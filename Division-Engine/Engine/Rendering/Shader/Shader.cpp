#include "Shader.h"
#include "../../Core/Debug/Debug.h"

Shader::Shader(const std::string& shaderPath_)
{
	// Read Shader filepath and compliles it
	std::string file;
	ReadFromFile(shaderPath_, file);

	if (!file.empty()) {
		RefShaderDefition(shaderPath_);
		shader = glCreateShader(GetGLShaderDef(shaderDef));
		const char* fileChar = file.c_str();
		glShaderSource(shader, 1, &fileChar, NULL);
		glCompileShader(shader);
		CheckCompilation(shader);
	}
	else {
		Debug::Error("No shader found on this file: " + shaderPath_, __FILE__, __LINE__);
	}
}

Shader::~Shader()
{
	OnDestroy();
}

void Shader::OnDestroy() {
	glDeleteShader(shader);
}


// Using ifstream, Read Shader File path name
void Shader::ReadFromFile(const std::string& filePath_, std::string& fileOut) const
{
	std::ifstream fileStream;
	fileStream.open(filePath_);

	std::string str;
	if (fileStream.is_open()) {
		while (std::getline(fileStream, str)) {
			fileOut += str + '\n';
		}
		fileStream.close();
	}
	else {
		Debug::Error("Cannot open shader file: " + filePath_, __FILE__, __LINE__);
	}
}

void Shader::CheckCompilation(const GLuint shader_) const
{
	std::cout << "\nShader " << shader_ << " Compilation Status \n";

	// Check Compile status
	GLint status;
	glGetShaderiv(shader_, GL_COMPILE_STATUS, &status);
	if (!status) {
		std::cout << "Failed to compile Shader\n";
	}
	else {
		std::cout << "Shader Compiled sucessfully!\n";
	}

	GLint bufflen;
	glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &bufflen);
	if (bufflen > 1) {
		GLchar* log = new char[bufflen + 1];
		glGetShaderInfoLog(shader_, bufflen, 0, log);
		std::cout << "Compilation log:\n";
		std::cout << log << std::endl;
		delete log;
	}
}


GLenum Shader::GetGLShaderDef(ShaderDefinition shaderDef_) const
{
	// Switch case statement for Shader definition using enum
	switch (shaderDef_)
	{
	case VERTEX:
		return GL_VERTEX_SHADER;
	case FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case TESSELLATION_CONTROL:
		return GL_TESS_CONTROL_SHADER;
	case TESSELLATION_EVALUATION:
		return GL_TESS_EVALUATION_SHADER;
	case GEOMETRY:
		return GL_GEOMETRY_SHADER;

	default:
		return -1;
	}
}


void Shader::RefShaderDefition(const std::string& shaderPath_)
{
	// string::npos -> Maximum value for size_t
	if (shaderPath_.find(".vs") != std::string::npos) {
		shaderDef = VERTEX;
	}
	else if (shaderPath_.find(".fs") != std::string::npos) {
		shaderDef = FRAGMENT;
	}
	else if (shaderPath_.find(".tes") != std::string::npos) {
		shaderDef = TESSELLATION_EVALUATION;
	}
	else if (shaderPath_.find(".tcs") != std::string::npos) {
		shaderDef = TESSELLATION_CONTROL;
	}
	else if (shaderPath_.find(".gs") != std::string::npos) {
		shaderDef = GEOMETRY;
	}
}
