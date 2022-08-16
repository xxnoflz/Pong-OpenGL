#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Shader.h"
#include "Texture.h"

class Renderer {
public:
	Renderer(Shader& shader);
	
	void Draw(glm::vec2 position, glm::vec2 size, Texture& texture, glm::vec2 spritePos, glm::vec2 spriteSize);
private:
	unsigned int m_VAO;
	GLuint m_vertex_VBO;
	GLuint m_texture_VBO;
	Shader m_shader;

	void Init();
};