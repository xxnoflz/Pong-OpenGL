#include "Shader.h"

Shader::Shader(const std::string vertexCode, const std::string fragmentCode) : ID() {
	this->CreateShader(vertexCode, fragmentCode);
}

void Shader::CreateShader(const std::string vertexCode, const std::string fragmentCode) {
	const char* vertex_c_str{ vertexCode.c_str() };
	unsigned int vertexShader{ glCreateShader(GL_VERTEX_SHADER) };
	glShaderSource(vertexShader, 1, &vertex_c_str, nullptr);
	glCompileShader(vertexShader);

	const char* fragment_c_str{ fragmentCode.c_str() };
	unsigned int fragmentShader{ glCreateShader(GL_FRAGMENT_SHADER) };
	glShaderSource(fragmentShader, 1, &fragment_c_str, nullptr);
	glCompileShader(fragmentShader);

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);
	glLinkProgram(this->ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() const {
	glUseProgram(this->ID);
}

void Shader::setMat4(const std::string name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}