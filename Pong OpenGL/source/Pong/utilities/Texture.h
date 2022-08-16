#pragma once
#include <glad/glad.h>

#include <vector>

class Texture {
public:
	Texture(const unsigned char* data = nullptr, int width = 0, int height = 0);

	void Bind() const;

	std::vector<float> GetFromTextureAtlas(float x, float y, float SpriteSizeX, float SpriteSizeY);
private:
	GLuint ID;
	int width;
	int height;

	void Init(const unsigned char* data);
};