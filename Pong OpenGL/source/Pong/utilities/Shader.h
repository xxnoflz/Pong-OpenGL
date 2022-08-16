#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>

class Shader {
public:
	Shader(std::string vertexCode = "", std::string fragmentCode = "");

	void use() const;

	void setMat4(std::string name, glm::mat4 value) const;
private:
	unsigned int ID;

	void CreateShader(std::string vertexCode, std::string fragmentCode);
};