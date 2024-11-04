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
	speed = 3;
	reloadTime = 60; // Reload time of 60 frame, or 1 sec.
	currentReloadTime = 0;
	directionChangeTime = (rand() % 300) + 100; // Direction change time of 1-3 seconds
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
		directionX = -directionX;
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
		Bullet* bullet = new Bullet(x + width / 2, y + 26 + height / 2, dx, dy, 7, Side::ENEMY_SIDE);
		bullets.push_back(bullet);
		getScene()->addGameObject(bullet);

		// After firing, reset our reload timer
		currentReloadTime = reloadTime;
	}

	// Bullet cleanup: remove bullets that are off the screen
	for (int i = 0; i < bullets.size(); i++)
	{
		// Check if the bullet is off the left side, right side, or below the screen
		if (bullets[i]->getPositionX() < 0 ||
			bullets[i]->getPositionX() > SCREEN_WIDTH ||
			bullets[i]->getPositionY() > SCREEN_HEIGHT)
		{
			// Cache the variable to be deleted later
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToErase;
			i--; // Adjust index to account for removal
		}
	}
}

void Enemy::draw()
{
	SDL_Rect destRect = { x, y, width, height };

	SDL_RenderCopyEx(app.renderer, texture, NULL, &destRect, -90, NULL, SDL_FLIP_NONE);
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