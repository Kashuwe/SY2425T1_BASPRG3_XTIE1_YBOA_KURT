#include "Bullet.h"


Bullet::Bullet(int positionX, int positionY, float directionX, float directionY, int speed, Side side)
{
	this->x = positionX;
	this->y = positionY;
	this->directionY = directionY;
	this->directionX = directionX;
	this->speed = speed;
	this->side = side;
	this->damage = (side == Side::PLAYER_SIDE) ? 10 : 1;
}

void Bullet::start()
{
	if (side == Side::PLAYER_SIDE)
		texture = loadTexture("gfx/playerBullet.png");
	else
		texture = loadTexture("gfx/alienBullet.png");

	width = 0;
	height = 0;

	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Bullet::update()
{
	x += directionX * speed;
	y += directionY * speed;
}

void Bullet::draw()
{
	 // Create destination rectangle for rendering the bullet
	SDL_Rect destRect = { x, y, width, height };

	// Determine the rotation angle based on the bullet's side
	double rotationAngle = 0;

	if (side == Side::PLAYER_SIDE)
	{
		// Player's bullet should face upwards
		rotationAngle = -90;
	}
	else if (side == Side::ENEMY_SIDE)
	{
		// Enemy's bullet should face downwards
		rotationAngle = 90;
	}

	// Render the bullet with the calculated rotation angle
	SDL_RenderCopyEx(app.renderer, texture, NULL, &destRect, rotationAngle, NULL, SDL_FLIP_NONE);
}

int Bullet::getPositionX()
{
	return x;
}

int Bullet::getPositionY()
{
	return y;
}

int Bullet::getWidth()
{
	return width;
}

int Bullet::getHeight()
{
	return height;
}

Side Bullet::getSide()
{
	return side;
}
