#include "Enemy.h"
#include "Scene.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::start()
{
	//load texture
	texture = loadTexture("gfx/enemy.png");

	//initial to avoid garbage values
	directionX = -1;
	directionY = 1;
	width = 0;
	height = 0;
	speed = 2;
	reloadTime = 60; // Reload time of 60 frame, or 1 sec.
	currentReloadTime = 0;
	directionChangeTime = (rand() % 300) + 180; // Direction change time of 3-8 seconds
	currentDirectionChangeTime = 0;

	// query the texture to set w&h 
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	// Load enemy firing sound
	sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
	sound->volume = 64;
}

void Enemy::update()
{	// Move
	x += directionX * speed;
	y += directionY * speed;

	// Basic AI, switch directions every X seconds
	if (currentDirectionChangeTime > 0)
		currentDirectionChangeTime--;

	if (currentDirectionChangeTime == 0)
	{
		// Flip directions
		directionY = -directionY;
		currentDirectionChangeTime = directionChangeTime;
	}

	// Decrement the enemy's reload timer
	if (currentReloadTime > 0) 
		currentReloadTime--;

	// Only fire when our reload timer is ready
	if (currentReloadTime == 0)
	{
		float dx = -1;
		float dy = 0;

		calcSlope(playerTarget->getPositionX(), playerTarget->getPositionY(), x, y, &dx, &dy);

		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet(x + width / -6, y - 4 + height / 2, dx, dy, 10, Side::ENEMY_SIDE);
		bullets.push_back(bullet);
		getScene()->addGameObject(bullet);


		// After firing, reset our reload timer
		currentReloadTime = reloadTime;
	}

	// Bullet cleanup: remove bullets that are off the screen
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionX() < 0)
		{
			// Cache the variable to be deleted later
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToErase;
			break; // Delete one bullet per frame

		}
	}
}

void Enemy::draw()
{
	blit(texture, x, y);
}

void Enemy::setPlayerTarget(Player* player)
{
	playerTarget = player;
}

void Enemy::setPosition(int xPos, int yPos)
{
	this->x = xPos;
	this->y = yPos;
}

int Enemy::getPositionX() const
{
	return x;
}

int Enemy::getPositionY() const
{
	return y;
}

int Enemy::getWidth() const
{
	return width;
}

int Enemy::getHeight() const
{
	return height;
}
