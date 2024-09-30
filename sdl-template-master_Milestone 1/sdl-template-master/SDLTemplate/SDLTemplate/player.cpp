#include "Player.h"

void Player::start()
{
	//load texture
	texture = loadTexture("gfx/player.png");

	//initial to avoid garbage values
	x = 100;
	y = 100;
	width = 0;
	height = 0;

	speed = 3;

	// query the texture to set w&h 
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}



void Player::update()
{
	if (app.keyboard[SDL_SCANCODE_W])
	{
		y -= speed;
	}

	if (app.keyboard[SDL_SCANCODE_S])
	{
		y += speed;
	}

	if (app.keyboard[SDL_SCANCODE_A])
	{
		x -= speed;
	}

	if (app.keyboard[SDL_SCANCODE_D])
	{
		x += speed;
	}

	if (app.keyboard[SDL_SCANCODE_LSHIFT])
	{
		speed = 5;
	}

	if (app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		speed = baseSpeed;
	}
}

void Player::draw()
{
	blit(texture, x, y);
}