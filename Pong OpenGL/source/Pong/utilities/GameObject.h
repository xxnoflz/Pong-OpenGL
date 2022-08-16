#pragma once
#include <glm/glm.hpp>

#include "Renderer.h"
#include "LineRenderer.h"

class GameObject {
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 velocity;

	GameObject();
	GameObject(glm::vec2 position, glm::vec2 size, glm::vec2 velocity);

	void Draw(Renderer* renderer, Texture& texture, glm::vec2 spritePos, glm::vec2 spriteSize);
	void DrawStrippedLine(LineRenderer* renderer, int count);

	virtual void Reset(glm::vec2 position);
};