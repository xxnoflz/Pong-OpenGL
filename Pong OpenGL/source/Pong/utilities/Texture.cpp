#include "Texture.h"

Texture::Texture(const unsigned char* data, int width, int height) : width(width), height(height) {
	this->Init(data);
}

void Texture::Bind() const {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture::Init(const unsigned char* data) {
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

std::vector<float> Texture::GetFromTextureAtlas(float x, float y, float SpriteSizeX, float SpriteSizeY) {
	return {
		(x * SpriteSizeX) / this->width,		(y * SpriteSizeY) / this->height,
		(x * SpriteSizeX) / this->width,		((y + 1) * SpriteSizeY) / this->height,
		((x + 1) * SpriteSizeX) / this->width,	(y * SpriteSizeY) / this->height,

		((x + 1) * SpriteSizeX) / this->width,	(y * SpriteSizeY) / this->height,
		((x + 1) * SpriteSizeX) / this->width,	((y + 1) * SpriteSizeY) / this->height,
		(x * SpriteSizeX) / this->width,		((y + 1) * SpriteSizeY) / this->height,
	};
}