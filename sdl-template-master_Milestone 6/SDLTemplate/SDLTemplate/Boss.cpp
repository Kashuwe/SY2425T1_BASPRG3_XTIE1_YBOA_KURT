#include "Boss.h"
#include "Scene.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

void Boss::start()
{
	//load texture
	texture = loadTexture("gfx/boss.png");

	// query the texture to set w&h 
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	//initial values
	x = (720 - width) / 2;
	y = 100;
	directionX = -1;
	width = 262;
	height = 204;
	speed = 2;

	// Boss specific properties
	maxHealth = 1000;
	currentHealth = maxHealth;

	// Attack timers
	bossReloadTime = 60; // Reload time of 60 frame, or 1 sec.
	bossCurrentReloadTime = 0;

	directionChangeTime = (rand() % 600) + 100; // Direction change time of 1-6 seconds
	currentDirectionChangeTime = 0;

	// Load enemy firing sound
	sound = SoundManager::loadSound("sound/334227__jradcoolness__laser.ogg");
	sound->volume = 86;

	isBossAlive = true;
}

void Boss::update()
{
	if (!isBossAlive) return;

	if (currentHealth <= 0) {
		// Handle boss death
		// You might want to trigger an explosion animation or victory condition
		return;
	}

	// Move
	x += directionX * speed;

	if (x < 0) { 
		x = 0;
		directionX = 1;
	}

	if (x > 720 - width) { 
		x = 720 - width;
		directionX = -1;
	}

	// Basic AI, switch directions every X seconds
	if (currentDirectionChangeTime > 0)
		currentDirectionChangeTime--;

	if (currentDirectionChangeTime == 0) {
		// Randomly flip direction
		directionX = -directionX;
		currentDirectionChangeTime = directionChangeTime;
	}

	// Decrement the enemy's reload timer
	if (bossCurrentReloadTime > 0)
		bossCurrentReloadTime--;

	// Only fire when our reload timer is ready
	if (bossCurrentReloadTime == 0)
	{
		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet(x + width / 2, y + 26 + height / 2, 0, 10, 7, Side::ENEMY_SIDE);
		bullets.push_back(bullet);
		getScene()->addGameObject(bullet);

		// After firing, reset our reload timer
		bossCurrentReloadTime = bossReloadTime;
	}

	// Bullet cleanup: remove bullets that are off the screen
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getPositionY() > SCREEN_HEIGHT) // Off-screen check
		{
			Bullet* bulletToErase = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToErase;
			break;
		}
	}
}

void Boss::draw()
{
	blit(texture, x, y);
}

void Boss::setPlayerTarget(Player* player)
{
	playerTarget = player;
}

void Boss::setPosition(int xPos, int yPos)
{
	this->x = xPos;
	this->y = yPos;
}

int Boss::getPositionX() const
{
	return x;
}

int Boss::getPositionY() const
{
	return y;
}

int Boss::getWidth() const
{
	return width;
}

int Boss::getHeight() const
{
	return height;
}

bool Boss::getIsBossAlive()
{
	return isBossAlive;
}

void Boss::doBossDeath()
{
	isBossAlive = false;
	speed = 0;
	bossCurrentReloadTime = INT_MAX; // Prevent further firing
}

void Boss::takeDamage(int damage)
{
	currentHealth -= damage;
	if (currentHealth < 0) currentHealth = 0;
	// add visual feedback here, like flashing the boss sprite
}

int Boss::getCurrentHealth() const
{
	return currentHealth;
}

int Boss::getMaxHealth() const
{
	return maxHealth;
}
