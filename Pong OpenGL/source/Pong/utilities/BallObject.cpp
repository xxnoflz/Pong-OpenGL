#include "BallObject.h"

BallObject::BallObject(const glm::vec2 position,const glm::vec2 size, const glm::vec2 velocity) : GameObject(position, size, velocity), stuck(true) { }

void BallObject::Move(const float dTime, const uint32_t window_height) {
	if (!this->stuck) {
		this->position += velocity * dTime;
	}
}

void BallObject::Reset(const glm::vec2 position, const uint32_t win_width, const uint32_t win_height) {
	this->position = glm::vec2((win_width / 2.0f) - (this->size.x / 2.0f), (win_height - this->position.y) - (this->size.y / 2.0f));
	this->stuck = true;
}