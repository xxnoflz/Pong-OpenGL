#include "Renderer.h"

Renderer::Renderer(Shader& shader) : m_shader(shader) {
	this->Init();
}

void Renderer::Init() {
	std::vector<float> verticesArray{
		0.0, 0.0,
		0.0, 1.0,
		1.0, 0.0,

		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0
	};

	glGenBuffers(1, &this->m_vertex_VBO);
	glGenVertexArrays(1, &this->m_VAO);

	glBindVertexArray(this->m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesArray.size() * sizeof(float), &verticesArray[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glGenBuffers(1, &this->m_texture_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->m_texture_VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::Draw(const glm::vec2 position, const glm::vec2 size, Texture& texture, const glm::vec2 spritePos, const glm::vec2 spriteSize) {
	texture.Bind();
	std::vector<float> textureArray{ texture.GetFromTextureAtlas(spritePos.x, spritePos.y, spriteSize.x, spriteSize.y) };
	glBindVertexArray(this->m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_texture_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float), &textureArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glm::mat4 model{ glm::mat4(1.0) };
	model = glm::translate(model, glm::vec3(position, 0.0));
	model = glm::scale(model, glm::vec3(size, 1.0));

	m_shader.use();
	m_shader.setMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
