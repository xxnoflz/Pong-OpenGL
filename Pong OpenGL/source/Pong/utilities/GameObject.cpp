#include "GameObject.h"

GameObject::GameObject() : position(), size(), velocity() { }

GameObject::GameObject(const glm::vec2 position, const glm::vec2 size, const glm::vec2 velocity) : position(position), size(size), velocity(velocity) { }

void GameObject::Draw(Renderer* renderer, Texture& texture, const glm::vec2 spritePos, const glm::vec2 spriteSize) {
	renderer->Draw(this->position, this->size, texture, spritePos, spriteSize);
}

void GameObject::DrawStrippedLine(LineRenderer* renderer, const int count) {
	renderer->DrawStrippedLine(this->position, this->size, count);
}

void GameObject::Reset(const glm::vec2 position) {
	this->position = position;
}