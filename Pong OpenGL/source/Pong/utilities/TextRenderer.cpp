#include "TextRenderer.h"

TextRenderer::TextRenderer(uint32_t width, uint32_t height) : VAO(), VBO(), FONT_FILE("source/Pong/Fonts/arial.ttf") {
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		return;
	FT_Face face;
	if (FT_New_Face(ft, FONT_FILE, 0, &face))
		return;
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c{}; c < 128; ++c) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			continue;
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Character character{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	this->Init(width, height);
}

void TextRenderer::Init(uint32_t width, uint32_t height) {
	TextShader = ResourceManager::GetShader("textRender");

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextRenderer::Render(std::string text, glm::vec2 position, float scale) {
	TextShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	for (std::string::const_iterator it{ text.begin() }; it != text.end(); ++it) {
		Character c{ Characters[*it] };
		float xPos{ position.x + c.Bearing.x * scale };
		float yPos{ position.y + (Characters['H'].Bearing.y - c.Bearing.y) * scale};

		float wSize{ c.Size.x * scale };
		float hSize{ c.Size.y * scale };
		float vertices[6][4]{
			xPos,			yPos + hSize,	0.0, 1.0,
			xPos + wSize,	yPos,			1.0, 0.0,
			xPos,			yPos,			0.0, 0.0,

			xPos,			yPos + hSize,	0.0, 1.0,
			xPos + wSize,   yPos + hSize,   1.0, 1.0,
			xPos + wSize,	yPos,			1.0, 0.0
		};
		glBindTexture(GL_TEXTURE_2D, c.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		position.x += (c.Advance >> 6) * scale;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}