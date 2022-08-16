#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <irrklang/irrKlang.h>

#include <cstdint>
#include <random>

#include "utilities/Renderer.h"
#include "utilities/ResourceManager.h"
#include "utilities/Player.h"
#include "utilities/BallObject.h"
#include "utilities/TextRenderer.h"
#include "utilities/LineRenderer.h"

const float PLAYER_PADDING_X(10.0f);
const float PLAYER_PADDING_Y(50.0f);

const glm::vec2 PLAYER_SIZE(10.0f, 110.0f);
const glm::vec2 PLAYER_VELOCITY(0.0f, 500.0f);
const glm::vec2 BALL_SIZE(15.0f);
const glm::vec2 BALL_VELOCITY(-500.0f, -300.0f);

struct TimeLogic {
	float deltaTime;
	float currentFrame;
	float lastFrame;
};

enum GameState {
	GAME_MENU,
	GAME_PLAY,
	GAME_PAUSE,
	GAME_END
};

class Pong {
public:
	Pong(uint32_t width, uint32_t height);

	void Init();
	void InitOpenGL();

	bool SelectAttackingPlayer();
	void UpdateTime();
	void InputHandler(float dTime);
	void Update(float dTime);

	bool CheckCollisions(Player& player, BallObject& ball);
	void DoCollision();

	void Render();

	void run();
private:
	GLFWwindow* m_window;
	Renderer* renderer;
	LineRenderer* line_render;
	TextRenderer* text;
	GameState state;
	irrklang::ISoundEngine* SoundEngine;

	GameObject* Line;
	Player* P1;
	Player* P2;

	Player* AttackingPlayer;
	Player* DefendingPlayer;
	BallObject* ball;

	const glm::vec2 INITIAL_P1_POS;
	const glm::vec2 INITIAL_P2_POS;
	const glm::vec2 INITIAL_BALL_POS;

	TimeLogic time;

	uint32_t m_height;
	uint32_t m_width;
};