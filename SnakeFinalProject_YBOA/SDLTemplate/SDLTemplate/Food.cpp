#include "Food.h"

void Food::start()
{
	width = 40;
	height = 40;

	// Load the power-up texture
	texture = loadTexture("gfx/apple.png");
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Food::draw()
{
	blit(texture, x, y);
}

void Food::update()
{
}

void Food::setPosition(int xPos, int yPos)
{
	this->x = (xPos / 40) * 40; // Snap to grid
	this->y = (yPos / 40) * 40;
}

int Food::getPositionX() const
{
	return x;
}

int Food::getPositionY() const
{
	return y;
}

int Food::getWidth() const
{
	return width;
}

int Food::getHeight() const
{
	return height;
}
