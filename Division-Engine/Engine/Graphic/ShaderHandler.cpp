#include "ShaderHandler.h"

// Redeclare static variable with Class ShaderHandler:: 
std::unique_ptr<ShaderHandler> ShaderHandler::shaderInstance = nullptr;
std::map<std::string, GLuint> ShaderHandler::programs = std::map<std::string, GLuint>();

ShaderHandler::ShaderHandler() {

}

ShaderHandler::~ShaderHandler() {
	OnDestroy();
}

ShaderHandler* ShaderHandler::GetInstance() {
	if (shaderInstance.get() == nullptr) {
		shaderInstance.reset(new ShaderHandler);
	}
	return shaderInstance.get();
}

void ShaderHandler::CreateProgram(const std::string& shaderName_, const std::string& vertexShaderFileName_,
	const std::string& fragmentShaderFileName_) {
	std::string vertexShaderCode = ReadShader(vertexShaderFileName_);
	std::string fragmentShaderCode = ReadShader(fragmentShaderFileName_);

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, shaderName_);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName_);

	GLint linkResult = 0;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (!linkResult) {
		GLint logLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programLog(logLength);
		glGetProgramInfoLog(program, logLength, NULL, &programLog[0]);
		std::string logString(programLog.begin(), programLog.end());
		Debug::Error("Error to link program " + shaderName_ + ". Error is:" + logString, "ShaderHandler.cpp", __LINE__);
		return;
	}
	programs[shaderName_] = program; // Go into the map see the shaderName_ if there is shaderKey it'll return to program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

const GLuint ShaderHandler::GetShader(const std::string& shaderName_) {
	if (programs.find(shaderName_) != programs.end()) {  // Create iterator finds the map array hold the pair of key value
		return programs[shaderName_]; // If program does exist this return the program key value;
	}
	return 0;
}

void ShaderHandler::OnDestroy() {
	if (programs.size() > 0) {
		for (auto entry : programs) {
			glDeleteProgram(entry.second);
		}
		programs.clear();
	}
}

std::string ShaderHandler::ReadShader(const std::string& fileName_) {
	std::string shaderCode;
	std::ifstream file;
	file.exceptions(std::iostream::badbit);
	try {
		file.open(fileName_.c_str());
		std::stringstream tmpStream;
		tmpStream << file.rdbuf();
		file.close();
		shaderCode = tmpStream.str();
	}
	catch (std::ifstream::failure error_) {
		Debug::Error("Could not read shader file:" + fileName_, "ShaderHandler.cpp", __LINE__);
		return "";
	}
	return shaderCode;
}

GLuint ShaderHandler::CreateShader(GLenum shaderType_, const std::string& source_, const std::string& shaderName_) {
	GLuint shader = glCreateShader(shaderType_);
	GLint compileResult = 0;
	const char* shaderCode = source_.c_str();
	const int shaderLength = source_.size();
	glShaderSource(shader, 1, &shaderCode, &shaderLength);

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (!compileResult) {
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> shaderLog(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, &shaderLog[0]);
		std::string logString(shaderLog.begin(), shaderLog.end());
		Debug::Error("Error compiling shader" + shaderName_ + ". Error is: " + logString, "ShaderHandler.cpp", __LINE__);
		return 0;
	}
	return shader;
}
