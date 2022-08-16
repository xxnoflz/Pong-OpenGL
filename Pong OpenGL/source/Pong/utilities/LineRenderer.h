#pragma once
#include <glad/glad.h>

#include <vector>

#include "Shader.h"

class LineRenderer {
public:
	LineRenderer(Shader& shader);

	void DrawStrippedLine(glm::vec2 position, glm::vec2 size, int count);
private:
	GLuint m_VAO;
	GLuint m_VBO;
	Shader m_shader;

	void Init();
};