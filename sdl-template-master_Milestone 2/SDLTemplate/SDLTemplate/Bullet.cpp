#include "Bullet.h"


Bullet::Bullet(float positionX, float positionY, float directionX, float directionY, float speed)
{
	this->x = positionX;
	this->y = positionY;
	this->directionY = directionY;
	this->directionX = directionX;
	this->speed = speed;
}

void Bullet::start()
{
	width = 0;
	height = 0;
	texture = loadTexture("gfx/playerBullet.png");

	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Bullet::update()
{
	x += directionX * speed;
	x += directionY * speed;
}

void Bullet::draw()
{
	blit(texture, x, y);
}

float Bullet::getPositonX()
{
	return x;
}

float Bullet::getPositionY()
{
	return y;
}

float Bullet::getWidth()
{
	return width;
}

float Bullet::getHeight()
{
	return height;
}
