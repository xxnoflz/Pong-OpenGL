#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"
#include "Shader.h"

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer {
public:
	TextRenderer(uint32_t width, uint32_t height);
	void Init(uint32_t width, uint32_t height);
	void Render(std::string text, glm::vec2 position, float scale);
private:
	const char* FONT_FILE;
	Shader TextShader;
	unsigned int VAO;
	unsigned int VBO;
	std::map<char, Character> Characters;
};