#pragma once
#include "GameObject.h"

class Player : public GameObject {
public:
	Player(glm::vec2 position, glm::vec2 size, glm::vec2 velocity);

	int score;
};