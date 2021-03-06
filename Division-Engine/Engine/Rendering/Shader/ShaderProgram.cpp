#include "ShaderProgram.h"


// Find ShaderProgram and Link through Shader class
ShaderProgram::ShaderProgram(const std::vector<Shader*>& shader_) : program(0)
{
	program = glCreateProgram();
	
	for (auto s : shader_) {
		glAttachShader(program, s->GetShader());
	}
	glLinkProgram(program);
	LinkStatus(program);
}

ShaderProgram::~ShaderProgram()
{
	OnDestroy();
}

void ShaderProgram::OnDestroy()
{
	glDeleteProgram(program);
}

void ShaderProgram::BindShader() const
{
	glUseProgram(program);
}

void ShaderProgram::UnBindShader() const
{
	glUseProgram(0);
}

void ShaderProgram::LinkStatus(const GLuint program_) const
{
	std::cout << "\n[" << program << "] ShaderProgram Link Status: " << std::endl;

	// Check Link Shaderprogram status
	GLint status;
	glGetProgramiv(program_, GL_LINK_STATUS, &status);
	
	if (!status) {
		std::cout << "[" << program << "] Failed!" << std::endl;
	}
	else {
		std::cout << "[" << program << "] Success!" << std::endl;;
	}

	//Check GL info log length status 
	GLint bufflen;
	glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &bufflen);

	if (bufflen > 1) {
		GLchar* log = new char[bufflen + 1];
		glGetProgramInfoLog(program_, bufflen, 0, log);
		std::cout << "Linked Log: \n";
		std::cout << log << std::endl;
		delete log;
	}
}
