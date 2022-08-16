#pragma once
#include "GameObject.h"

class BallObject : public GameObject {
public:
	BallObject(glm::vec2 position, glm::vec2 size, glm::vec2 velocity);

	void Move(float dTime, uint32_t window_height);

	void Reset(glm::vec2 position, uint32_t win_width, uint32_t win_height);

	bool stuck;
};