#include "Pong.h"

Pong::Pong(uint32_t width, uint32_t height) : m_width(width), m_height(height), state(GAME_MENU),
INITIAL_P1_POS(0.0f + PLAYER_PADDING_X, height / 2.0f - PLAYER_PADDING_Y),
INITIAL_P2_POS(width - PLAYER_SIZE.x - PLAYER_PADDING_X, height / 2.0f - PLAYER_PADDING_Y),
INITIAL_BALL_POS(width / 2.0f - BALL_SIZE.x / 2.0f, height / 2.0f - BALL_SIZE.y / 2.0f) {
	this->Init();
}

void Pong::InitOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, "Pong OpenGL", nullptr, nullptr);
	if (m_window == nullptr)
		return;
	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return;
	glViewport(0, 0, m_width, m_height);
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool Pong::SelectAttackingPlayer() {
	std::random_device rd;
	std::mt19937 gen(rd());
	return bool(std::uniform_int_distribution<>{0, 1}(gen));
}

void Pong::Init() {
	this->InitOpenGL();
	ResourceManager::CreateShader("source/Pong/Shaders/object_render.vert", "source/Pong/Shaders/object_render.frag", "objectShader");
	ResourceManager::CreateShader("source/Pong/Shaders/text_render.vert", "source/Pong/Shaders/text_render.frag", "textRender");
	ResourceManager::CreateShader("source/Pong/Shaders/line_render.vert", "source/Pong/Shaders/line_render.frag", "lineRender");
	glm::mat4 projection{ glm::ortho(0.0f, (float)m_width, (float)m_height, 0.0f, -1.0f, 1.0f) };
	ResourceManager::GetShader("objectShader").use();
	ResourceManager::GetShader("objectShader").setMat4("projection", projection);
	ResourceManager::GetShader("textRender").use();
	ResourceManager::GetShader("textRender").setMat4("projection", projection);
	ResourceManager::GetShader("lineRender").use();
	ResourceManager::GetShader("lineRender").setMat4("projection", projection);

	ResourceManager::CreateTexture("source/Pong/Textures/sprites.png", "sprite");

	renderer = new Renderer(ResourceManager::GetShader("objectShader"));
	text = new TextRenderer(this->m_width, this->m_height);
	line_render = new LineRenderer(ResourceManager::GetShader("lineRender"));
	SoundEngine = irrklang::createIrrKlangDevice();

	P1 = new Player(INITIAL_P1_POS, PLAYER_SIZE, PLAYER_VELOCITY);
	P2 = new Player(INITIAL_P2_POS, PLAYER_SIZE, PLAYER_VELOCITY);
	bool SelectPlayer{ SelectAttackingPlayer() };
	DefendingPlayer = SelectPlayer ? P1 : P2;
	AttackingPlayer = !SelectPlayer ? P1 : P2;
	ball = new BallObject(INITIAL_BALL_POS, BALL_SIZE, (SelectPlayer) ? BALL_VELOCITY : -BALL_VELOCITY);
	Line = new GameObject(glm::vec2(this->m_width / 2.0f, 0.0f), glm::vec2(0.0, 10.0f), glm::vec2(0.0));
}

bool Pong::CheckCollisions(Player& player, BallObject& ball) {
	bool collisionX{ player.position.x + player.size.x >= ball.position.x &&
		ball.position.x + ball.size.x >= player.position.x };
	bool collisionY{ player.position.y + player.size.y >= ball.position.y &&
		ball.position.y + ball.size.y >= player.position.y };
	return collisionX && collisionY;
}

void Pong::DoCollision() {
	//Wall collisions
	if (ball->position.y <= 0) {
		ball->position.y = 0;
		ball->velocity.y = -ball->velocity.y;
		SoundEngine->play2D("source/Pong/Sounds/pong_wall.ogg");
	}
	else if (ball->position.y + ball->size.y >= this->m_height) {
		ball->position.y = this->m_height - ball->size.y;
		ball->velocity.y = -ball->velocity.y;
		SoundEngine->play2D("source/Pong/Sounds/pong_wall.ogg");
	}

	//Paddles collisions
	if (!ball->stuck && CheckCollisions(*DefendingPlayer, *ball)) {
		float centerBoard{ DefendingPlayer->position.y + DefendingPlayer->size.y / 2.0f };
		float distance{ (ball->position.y + (ball->size.y / 2.0f)) - centerBoard };
		float percentage{ distance / (DefendingPlayer->size.y / 2.0f) };

		ball->velocity.x = -ball->velocity.x;
		ball->velocity.y = std::abs(ball->velocity.x) * percentage;

		Player* tmp{ AttackingPlayer };
		AttackingPlayer = DefendingPlayer;
		DefendingPlayer = tmp;

		SoundEngine->play2D("source/Pong/Sounds/pong_paddle.ogg");
	}
}

void Pong::UpdateTime() {
	time.currentFrame = (float)glfwGetTime();
	time.deltaTime = time.currentFrame - time.lastFrame;
	time.lastFrame = time.currentFrame;
}

void Pong::InputHandler(float dTime) {
	//Game input
	if (glfwGetKey(this->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->m_window, true);
	if (state == GAME_MENU)
		if (glfwGetKey(this->m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			state = GAME_PLAY;

	//Player 1
	if (state == GAME_PLAY || state == GAME_PAUSE) {
		if (glfwGetKey(this->m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			ball->stuck = false;
			state = GAME_PLAY;
		}

		if (glfwGetKey(this->m_window, GLFW_KEY_W) == GLFW_PRESS)
			if (P1->position.y >= 0.0f)
				P1->position.y -= PLAYER_VELOCITY.y * dTime;
		if (glfwGetKey(this->m_window, GLFW_KEY_S) == GLFW_PRESS)
			if (P1->position.y <= this->m_height - P1->size.y)
				P1->position.y += PLAYER_VELOCITY.y * dTime;
		//Player 2
		if (glfwGetKey(this->m_window, GLFW_KEY_UP) == GLFW_PRESS)
			if (P2->position.y >= 0.0f)
				P2->position.y -= PLAYER_VELOCITY.y * dTime;
		if (glfwGetKey(this->m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
			if (P2->position.y <= this->m_height - P2->size.y)
				P2->position.y += PLAYER_VELOCITY.y * dTime;
	}

	if(state == GAME_END)
		if (glfwGetKey(this->m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			P1 = new Player(INITIAL_P1_POS, PLAYER_SIZE, PLAYER_VELOCITY);
			P2 = new Player(INITIAL_P2_POS, PLAYER_SIZE, PLAYER_VELOCITY);
			bool SelectPlayer{ SelectAttackingPlayer() };
			DefendingPlayer = SelectPlayer ? P1 : P2;
			AttackingPlayer = !SelectPlayer ? P1 : P2;
			ball = new BallObject(INITIAL_BALL_POS, BALL_SIZE, (SelectPlayer) ? BALL_VELOCITY : -BALL_VELOCITY);
			state = GAME_MENU;
		}
}

void Pong::Update(float dTime) {
	ball->Move(dTime, this->m_height);

	this->DoCollision();

	if (ball->position.x <= 0 || ball->position.x >= this->m_width - ball->size.x) {
		P1->Reset(INITIAL_P1_POS);
		P2->Reset(INITIAL_P2_POS);
		ball->Reset(INITIAL_BALL_POS, this->m_width, this->m_height);
		state = GAME_PAUSE;

		++AttackingPlayer->score;

		SoundEngine->play2D("source/Pong/Sounds/pong_score.ogg");
	}

	if (P1->score == 10 || P2->score == 10)
		state = GAME_END;
}

void Pong::Render() {
	if (state == GAME_MENU) {
		text->Render("Welcome to Pong OpenGL", glm::vec2(this->m_width / 2.0f - 125.0, this->m_height / 2.0f - 100.0f), 0.5);
		text->Render("Press SPACE to start", glm::vec2(this->m_width / 2.0f - 100.0, this->m_height / 2.0f - 50.0f), 0.5);
	}
	if (state == GAME_PLAY || state == GAME_MENU || state == GAME_END || state == GAME_PAUSE) {
		Line->DrawStrippedLine(line_render, this->m_height / (int)Line->size.y);
		P1->Draw(renderer, ResourceManager::GetTexture("sprite"), glm::vec2(0, 0), glm::vec2(50, 150));
		P2->Draw(renderer, ResourceManager::GetTexture("sprite"), glm::vec2(1, 0), glm::vec2(50, 150));
		ball->Draw(renderer, ResourceManager::GetTexture("sprite"), glm::vec2(2, 0), glm::vec2(50, 50));
	}
	if (state == GAME_PLAY || state == GAME_PAUSE || state == GAME_END) {
		text->Render(std::to_string(P1->score), glm::vec2(50.0, 10.0), 1);
		text->Render(std::to_string(P2->score), glm::vec2(this->m_width - 50.0, 10.0), 1);
	}
	if (state == GAME_PAUSE) 
		text->Render("Press SPACE to continue", glm::vec2(this->m_width / 2.0f - 130.0, this->m_height / 2.0f - 50.0f), 0.5);
	if(state == GAME_END)
		text->Render("Press SPACE to restart or ESC to close", glm::vec2(this->m_width / 2.0f - 200.0, this->m_height / 2.0f - 50.0f), 0.5);
}

void Pong::run() {
	while (!glfwWindowShouldClose(m_window)) {
		UpdateTime();
		InputHandler(time.deltaTime);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		Update(time.deltaTime);

		Render();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	glfwTerminate();
}