#include "LineRenderer.h"

LineRenderer::LineRenderer(Shader& shader) : m_shader(shader) {
	this->Init();
}

void LineRenderer::Init() {
	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glBindVertexArray(this->m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineRenderer::DrawStrippedLine(const glm::vec2 position, const glm::vec2 size, const int count) {
	m_shader.use();
	glBindVertexArray(this->m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	for (int it{}; it < count; ++it) {
		std::vector<float> vertices{
			position.x, position.y + (size.y * 2 * it),
			position.x, position.y + (size.y * 2 * it) + size.y
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), &vertices[0]);
		glDrawArrays(GL_LINES, 0, 2);
	}
}
