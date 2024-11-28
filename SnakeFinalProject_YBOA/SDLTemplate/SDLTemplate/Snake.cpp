#include "Snake.h"

Snake::Snake()
{
	start();
}

Snake::~Snake()
{
	// DESTROYYY!
	SDL_DestroyTexture(tail_left);
	SDL_DestroyTexture(tail_up);
	SDL_DestroyTexture(tail_down);
	SDL_DestroyTexture(tail_right);
	SDL_DestroyTexture(body_bottomright);
	SDL_DestroyTexture(body_bottomleft);
	SDL_DestroyTexture(body_topleft);
	SDL_DestroyTexture(body_topright);
	SDL_DestroyTexture(body_horizontal);
	SDL_DestroyTexture(body_vertical);
	SDL_DestroyTexture(head_down);
	SDL_DestroyTexture(head_right);
	SDL_DestroyTexture(head_left);
	SDL_DestroyTexture(head_up);
}

void Snake::start()
{
	// Clear vector
	snake.clear();

	// starting pos snekk
	snake.push_back({ 120, 320, 40, 40 }); // Head
	snake.push_back({ 80, 320, 40, 40 });  // Body
	snake.push_back({ 40, 320, 40, 40 });  // Tail

	// initial direction and state
	direction = { 40, 0 }; // Moving right
	isGameOver = false;

	// eto
	width = 40;
	height = 40;

	// head position
	headX = snake[0].x;
	headY = snake[0].y;

	//Load textures
	//DAMIII
	tail_left = loadTexture("gfx/tail_left.png");
	tail_up = loadTexture("gfx/tail_up.png");
	tail_down = loadTexture("gfx/tail_down.png");
	tail_right = loadTexture("gfx/tail_right.png");
	body_bottomright = loadTexture("gfx/body_bottomright.png");
	body_bottomleft = loadTexture("gfx/body_bottomleft.png");
	body_topleft = loadTexture("gfx/body_topleft.png");
	body_topright = loadTexture("gfx/body_topright.png");
	body_horizontal = loadTexture("gfx/body_horizontal.png");
	body_vertical = loadTexture("gfx/body_vertical.png");
	head_down = loadTexture("gfx/head_down.png");
	head_right = loadTexture("gfx/head_right.png");
	head_left = loadTexture("gfx/head_left.png");
	head_up = loadTexture("gfx/head_up.png");
}

void Snake::draw(SDL_Renderer* renderer)
{
	// complicated but yes
	for (int i = 0; i < snake.size(); i++) {
		if (i == 0) {
			if (snake[i].x > snake[i + 1].x) {
				SDL_RenderCopy(app.renderer, head_right, NULL, &snake[i]);
			}
			else if (snake[i].x < snake[i + 1].x) {
				SDL_RenderCopy(app.renderer, head_left, NULL, &snake[i]);
			}
			else if (snake[i].y > snake[i + 1].y) {
				SDL_RenderCopy(app.renderer, head_down, NULL, &snake[i]);
			}
			else {
				SDL_RenderCopy(app.renderer, head_up, NULL, &snake[i]);
			}
		}
		else if (i == snake.size() - 1) {
			if (snake[i].x > snake[i - 1].x) {
				SDL_RenderCopy(app.renderer, tail_right, NULL, &snake[i]);
			}
			else if (snake[i].x < snake[i - 1].x) {
				SDL_RenderCopy(app.renderer, tail_left, NULL, &snake[i]);
			}
			else if (snake[i].y > snake[i - 1].y) {
				SDL_RenderCopy(app.renderer, tail_down, NULL, &snake[i]);
			}
			else {
				SDL_RenderCopy(app.renderer, tail_up, NULL, &snake[i]);
			}
		}
		else {
			int x_previous = snake[i - 1].x - snake[i].x;
			int y_previous = snake[i - 1].y - snake[i].y;
			int x_next = snake[i].x - snake[i + 1].x;
			int y_next = snake[i].y - snake[i + 1].y;
			if (snake[i - 1].x == snake[i + 1].x) {
				SDL_RenderCopy(app.renderer, body_vertical, NULL, &snake[i]);
			}
			else if (snake[i - 1].y == snake[i + 1].y) {
				SDL_RenderCopy(app.renderer, body_horizontal, NULL, &snake[i]);
			}
			else {
				if (x_previous == -40 && y_next == 40 || y_previous == -40 && x_next == 40){
					SDL_RenderCopy(app.renderer, body_topleft, NULL, &snake[i]);
				}
				else if (x_previous == -40 && y_next == -40 || y_previous == 40 && x_next == 40) {
					SDL_RenderCopy(app.renderer, body_bottomleft, NULL, &snake[i]);
				}
				else if (x_previous == 40 && y_next == 40 || y_previous == -40 && x_next == -40) {
					SDL_RenderCopy(app.renderer, body_topright, NULL, &snake[i]);
				}
				else{
					SDL_RenderCopy(app.renderer, body_bottomright, NULL, &snake[i]);
				}
			}
		}
	}
}

void Snake::update()
{
	// GameOver na ba? 
	if (isGameOver) {
		return; 
	}

	//Movement Controls
	// Movement Controls: Only allow direction change if it's not the opposite direction
	if (app.keyboard[SDL_SCANCODE_UP] && direction.y != 40 && lastDirectionChangeTime + directionChangeCooldown <= SDL_GetTicks()) {
		direction.x = 0;
		direction.y = -40;
		lastDirectionChangeTime = SDL_GetTicks(); 
	}
	else if (app.keyboard[SDL_SCANCODE_DOWN] && direction.y != -40 && lastDirectionChangeTime + directionChangeCooldown <= SDL_GetTicks()) {
		direction.x = 0;
		direction.y = 40;
		lastDirectionChangeTime = SDL_GetTicks();
	}
	else if (app.keyboard[SDL_SCANCODE_RIGHT] && direction.x != -40 && lastDirectionChangeTime + directionChangeCooldown <= SDL_GetTicks()) {
		direction.x = 40;
		direction.y = 0;
		lastDirectionChangeTime = SDL_GetTicks(); 
	}
	else if (app.keyboard[SDL_SCANCODE_LEFT] && direction.x != 40 && lastDirectionChangeTime + directionChangeCooldown <= SDL_GetTicks()) {
		direction.x = -40;
		direction.y = 0;
		lastDirectionChangeTime = SDL_GetTicks(); 
	}

	Uint32 currentTick = SDL_GetTicks();  // time in milliseconds
	Uint32 deltaTime = currentTick - lastMoveTime; // time difference between frames

	if (deltaTime >= moveDelay) {
		{  
			move();  // Move the snake
		}
		lastMoveTime = currentTick; // Update next for next movement
	}
}

void Snake::move()
{
	// Shift body segments to follow the head
	for (int i = snake.size() - 1; i > 0; --i) {
		snake[i] = snake[i - 1];
	}

	// Move the head
	snake[0].x += direction.x;
	snake[0].y += direction.y;

	// Move the head based on the direction
	snake[0].x = (snake[0].x / 40) * 40;
	snake[0].y = (snake[0].y / 40) * 40;

	// CCurrent position of the head
	headX = snake[0].x;
	headY = snake[0].y;
}

void Snake::grow()
{
	// Get the last segment (tail)
	SDL_Rect newSegment = snake.back();  // The last segment
	// Add segment tail position
	snake.push_back(newSegment);  // add the new body part
}

void Snake::doDeath()
{
	isGameOver = true;
}

int Snake::getHeadX() const
{
	return headX;
}

int Snake::getHeadY() const
{
	return headY;
}

int Snake::getWidth() const
{
	return width;
}

int Snake::getHeight() const
{
	return height;
}

void Snake::reset()
{
	start();
}

int Snake::getBodySize() const
{
	return snake.size();
}

int Snake::getBodySegmentX(int index) const
{
	if (index >= 0 && index < snake.size()) {
		return snake[index].x;
	}
	return -1; 
}

int Snake::getBodySegmentY(int index) const
{
	if (index >= 0 && index < snake.size()) {
		return snake[index].y;
	}
	return -1; 
}